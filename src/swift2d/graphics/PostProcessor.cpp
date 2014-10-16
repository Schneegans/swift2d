////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/PostProcessor.hpp>

#include <swift2d/settings/SettingsWrapper.hpp>
#include <swift2d/materials/ShaderIncludes.hpp>
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/application/Paths.hpp>
#include <swift2d/databases/TextureDatabase.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PostProcessor::PostProcessor(RenderContext const& ctx)
  : post_fx_shader_()
  , threshold_shader_(R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2 position;
      uniform ivec2 screen_size;

      out vec2 texcoord1;
      out vec2 texcoord2;
      out vec2 texcoord3;
      out vec2 texcoord4;

      void main(void){
        texcoord1 = vec2(position.x + 1.0, 1.0 + position.y) * 0.5;
        texcoord2 = texcoord1 + vec2(0.5/screen_size.x, 0.0);
        texcoord3 = texcoord1 + vec2(0.0, 0.5/screen_size.y);
        texcoord4 = texcoord1 + vec2(0.5/screen_size.x, 0.5/screen_size.y);
        gl_Position = vec4(position, 0.0, 1.0);
      }
    )", R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      uniform sampler2D g_buffer_diffuse;
      uniform sampler2D g_buffer_light;
      uniform ivec2 screen_size;

      in vec2 texcoord1;
      in vec2 texcoord2;
      in vec2 texcoord3;
      in vec2 texcoord4;

      layout (location = 0) out vec3 fragColor;

      void main(void) {
        vec3 color = texture2D(g_buffer_diffuse, texcoord1).rgb
                   + texture2D(g_buffer_diffuse, texcoord2).rgb
                   + texture2D(g_buffer_diffuse, texcoord3).rgb
                   + texture2D(g_buffer_diffuse, texcoord4).rgb;

        float glow = texture2D(g_buffer_light, texcoord1).b
                   + texture2D(g_buffer_light, texcoord2).b
                   + texture2D(g_buffer_light, texcoord3).b
                   + texture2D(g_buffer_light, texcoord4).b;

        fragColor = pow(glow * color / 16.0, vec3(2));
      }
    )")
  , g_buffer_diffuse_(post_fx_shader_.get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_normal_(post_fx_shader_.get_uniform<int>("g_buffer_normal"))
  , screen_size_(post_fx_shader_.get_uniform<math::vec2i>("screen_size"))
  , l_buffer_(post_fx_shader_.get_uniform<int>("l_buffer"))
  , glow_buffers_(post_fx_shader_.get_uniform<int>("glow_buffers"))
  , heat_buffer_(post_fx_shader_.get_uniform<int>("heat_buffer"))
  , dirt_tex_(post_fx_shader_.get_uniform<int>("dirt_tex"))
  , dirt_opacity_(post_fx_shader_.get_uniform<float>("dirt_opacity"))
  , use_heat_(post_fx_shader_.get_uniform<int>("use_heat"))
  , gamma_(post_fx_shader_.get_uniform<float>("gamma"))
  , vignette_color_(post_fx_shader_.get_uniform<math::vec4>("vignette_color"))
  , vignette_softness_(post_fx_shader_.get_uniform<float>("vignette_softness"))
  , vignette_coverage_(post_fx_shader_.get_uniform<float>("vignette_coverage"))
  , use_color_grading_(post_fx_shader_.get_uniform<int>("use_color_grading"))
  , color_grading_tex_(post_fx_shader_.get_uniform<int>("color_grading_tex"))
  , color_grading_intensity_(post_fx_shader_.get_uniform<float>("color_grading_intensity"))
  , screen_size_threshold_(threshold_shader_.get_uniform<math::vec2i>("screen_size"))
  , g_buffer_diffuse_threshold_(threshold_shader_.get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_light_(threshold_shader_.get_uniform<int>("g_buffer_light"))
  , streak_effect_(ctx)
  , ghost_effect_(ctx)
  , heat_effect_(ctx)
  , dirt_(Paths::get().resource("images", "dirt.jpg"))
 {

  std::string v_source(R"(
    // vertex shader -----------------------------------------------------------
    @include "fullscreen_quad_vertex_shader"
  )");

  std::stringstream f_source;
  f_source << R"(
    // fragment shader ---------------------------------------------------------
    @include "version"

    in vec2 texcoords;
    uniform ivec2 screen_size;
    @include "gbuffer_input"

    layout (location = 0) out vec3 fragColor;

    // vignetting --------------------------------------------------------------
    uniform vec4  vignette_color;
    uniform float vignette_coverage;
    uniform float vignette_softness;

    vec3 apply_vignette(vec3 color_in) {
      // inigo quilez's great vigneting effect!
      float a = -vignette_coverage/vignette_softness;
      float b = 1.0/vignette_softness;
      vec2 q = texcoords;
      float fac = clamp(a + b*pow( 16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.1 ), 0, 1) * vignette_color.a + (1-vignette_color.a);

      return mix(vignette_color.rgb, color_in, fac);
    }

    // color grading -----------------------------------------------------------
    uniform sampler3D color_grading_tex;
    uniform bool      use_color_grading;
    uniform float     color_grading_intensity;

    vec3 apply_color_grading(vec3 color_in) {
      if (use_color_grading) {
        return mix(color_in, texture(color_grading_tex, color_in).xyz, color_grading_intensity);
      }
      return color_in;
    }

    // gamma correction --------------------------------------------------------
    uniform float gamma;

    vec3 apply_gamma(vec3 color_in) {
      return pow(color_in, 1.0/vec3(gamma));
    }
  )";

  // if (ctx.light_sub_sampling) {
  //   f_source << R"(
  //     // lbuffer sampling ------------------------------------------------------
  //     uniform sampler2D l_buffer;

  //     vec3 get_lighting(vec2 texcoords) {

  //       int level = 3;

  //       vec2 coords = gl_FragCoord.xy;
  //       vec2 light_coord_00 = (coords + vec2(-level, -level))/screen_size;
  //       vec2 light_coord_01 = (coords + vec2(-level,  level))/screen_size;
  //       vec2 light_coord_10 = (coords + vec2( level, -level))/screen_size;
  //       vec2 light_coord_11 = (coords + vec2( level,  level))/screen_size;

  //       vec3 normal    = get_normal(texcoords);
  //       vec3 normal_00 = get_normal(light_coord_00);
  //       vec3 normal_01 = get_normal(light_coord_01);
  //       vec3 normal_10 = get_normal(light_coord_10);
  //       vec3 normal_11 = get_normal(light_coord_11);

  //       float fac_00 = max(0, dot(normal, normal_00));
  //       float fac_01 = max(0, dot(normal, normal_01));
  //       float fac_10 = max(0, dot(normal, normal_10));
  //       float fac_11 = max(0, dot(normal, normal_11));

  //       vec2 lookup = light_coord_00 * fac_00 + light_coord_01 * fac_01 +
  //                     light_coord_10 * fac_10 + light_coord_11 * fac_11;

  //       lookup = lookup / (fac_00 + fac_01 + fac_10 + fac_11);

  //       vec4 light = texture2D(l_buffer, lookup*0.001 + texcoords);
  //       return light.rgb * texture2D(g_buffer_diffuse, texcoords).rgb + light.a * light.rgb;
  //     }
  //   )";
  // } else {
    f_source << R"(
      // lbuffer sampling ------------------------------------------------------
      uniform sampler2D l_buffer;

      vec3 get_lighting(vec2 texcoords) {
        vec4 light = texture2D(l_buffer, texcoords);
        return light.rgb * texture2D(g_buffer_diffuse, texcoords).rgb + light.a * light.rgb;
      }
    )";
  // }

  if (!ctx.dynamic_lighting) {
    f_source << R"(
      vec3 get_color(vec2 texcoords) {
        return get_diffuse(texcoords);
      }
    )";
  } else if (!ctx.lens_flares) {
    f_source << R"(
      vec3 get_color(vec2 texcoords) {
        return get_lighting(texcoords);
      }
    )";
  } else {
    f_source << R"(

      uniform sampler2D glow_buffers[8];
      uniform sampler2D heat_buffer;
      uniform sampler2D dirt_tex;
      uniform float     dirt_opacity;
      uniform bool      use_heat;

      vec3 get_color(vec2 texcoords) {

        vec3 glow = vec3(0);
        for (int i=0; i<8; ++i) {
          glow += texture2D(glow_buffers[i], texcoords).rgb;
        }

        vec3 dirt = texture2D(dirt_tex, texcoords).rgb;

        vec2 shifted_texcoords = texcoords;
        if (use_heat) {
          vec2 offset = (texture2D(heat_buffer, texcoords).rg - 0.5) * 0.2;
          shifted_texcoords   += offset;
        }

        vec3 output = get_lighting(shifted_texcoords);
        output = output + glow * dirt * dirt_opacity;
        return output;
      }
    )";
  }

  f_source << R"(
    void main(void){
      vec3 output = get_color(texcoords);
      output = apply_color_grading(output);
      output = apply_vignette(output);
      output = apply_gamma(output);

      fragColor = output;
    }
  )";

  post_fx_shader_.set_sources(v_source, f_source.str());



  // helper lambda -------------------------------------------------------------
  auto create_texture = [&](
    oglplus::Texture& tex, int width, int height,
    oglplus::enums::PixelDataInternalFormat i_format,
    oglplus::enums::PixelDataFormat         p_format) {

    ctx.gl.Bound(oglplus::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
               0, p_format, oglplus::PixelDataType::UnsignedByte, nullptr)
      .MinFilter(oglplus::TextureMinFilter::Linear)
      .MagFilter(oglplus::TextureMagFilter::Linear)
      .WrapS(oglplus::TextureWrap::MirroredRepeat)
      .WrapT(oglplus::TextureWrap::MirroredRepeat);
  };

  // threshold members ---------------------------------------------------------
  auto size(ctx.g_buffer_size/6);

  create_texture(
    threshold_buffer_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGB8,
    oglplus::PixelDataFormat::RGB
  );

  threshold_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 0, threshold_buffer_, 0
  );
}

////////////////////////////////////////////////////////////////////////////////

void PostProcessor::process(ConstSerializedScenePtr const& scene, RenderContext const& ctx,
                            GBuffer* g_buffer, LBuffer* l_buffer) {

  auto use_postfx_shader = [&]() {
    post_fx_shader_.use(ctx);
    vignette_softness_.Set(scene->vignette_softness);
    vignette_coverage_.Set(scene->vignette_coverage);
    vignette_color_.Set(scene->vignette_color);

    TexturePtr color_map;

    if (scene->color_map_name != "") {
      color_map = TextureDatabase::get().lookup(scene->color_map_name);
    }

    color_grading_tex_.Set(1);

    if (color_map) {
      color_map->bind(ctx, 1);
      use_color_grading_.Set(1);
      color_grading_intensity_.Set(scene->color_grading_intensity);
    } else {
      use_color_grading_.Set(0);
      color_grading_intensity_.Set(0.f);
    }
  };

  if (!ctx.dynamic_lighting) {

    ctx.gl.Disable(oglplus::Capability::Blend);

    SWIFT_PUSH_GL_RANGE("Composite");

    ctx.gl.Viewport(ctx.window_size.x(), ctx.window_size.y());
    oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);

    g_buffer->bind_diffuse(0);

    use_postfx_shader();

    g_buffer_diffuse_.Set(0);
    gamma_.Set(SettingsWrapper::get().Settings->Gamma());
    Quad::get().draw(ctx);

    SWIFT_POP_GL_RANGE();

    ctx.gl.Enable(oglplus::Capability::Blend);

  } else {

    if (ctx.heat_effect) {
      SWIFT_PUSH_GL_RANGE("Heat");
      heat_effect_.process(scene, ctx);
      SWIFT_POP_GL_RANGE();
    }

    ctx.gl.Disable(oglplus::Capability::Blend);

    g_buffer->bind_diffuse(0);
    g_buffer->bind_light(1);
    g_buffer->bind_normal(2);
    l_buffer->bind(3);

    if (ctx.heat_effect) {
      // thresholding
      SWIFT_PUSH_GL_RANGE("Thresholding");
      generate_threshold_buffer(ctx);
      SWIFT_POP_GL_RANGE();

      // streaks
      SWIFT_PUSH_GL_RANGE("Streaks");
      streak_effect_.process(ctx, threshold_buffer_);
      SWIFT_POP_GL_RANGE();

      // ghosts
      SWIFT_PUSH_GL_RANGE("Ghosts");
      ghost_effect_.process(ctx, threshold_buffer_);
      SWIFT_POP_GL_RANGE();
    }

    SWIFT_PUSH_GL_RANGE("Composite");
    ctx.gl.Viewport(ctx.window_size.x(), ctx.window_size.y());
    oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);

    use_postfx_shader();

    g_buffer_diffuse_.Set(0);

    if (ctx.light_sub_sampling) {
      // g_buffer_normal_.Set(2);
      // screen_size_.Set(ctx.window_size);
    }
    l_buffer_.Set(3);
    gamma_.Set(SettingsWrapper::get().Settings->Gamma());

    if (ctx.lens_flares) {
      int start(4);
      start = streak_effect_.bind_buffers(start, ctx);
      start = ghost_effect_.bind_buffers(start, ctx);

      std::vector<int> units = {4, 5, 6, 7, 8, 9, 10, 11};
      glow_buffers_.Set(units);

      if (ctx.heat_effect) {
        heat_buffer_.Set(start);
        start = heat_effect_.bind_buffers(start, ctx);
        use_heat_.Set(1);
      } else {
        use_heat_.Set(0);
      }

      dirt_.bind(ctx, start);
      dirt_tex_.Set(start);
      dirt_opacity_.Set(scene->dirt_opacity);
    }

    Quad::get().draw(ctx);

    SWIFT_POP_GL_RANGE();

    ctx.gl.Enable(oglplus::Capability::Blend);
  }
}

////////////////////////////////////////////////////////////////////////////////

void PostProcessor::generate_threshold_buffer(RenderContext const& ctx) {

  auto size(ctx.g_buffer_size);

  ctx.gl.Viewport(size.x()/6, size.y()/6);

  threshold_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);

  threshold_shader_.use(ctx);
  g_buffer_diffuse_threshold_.Set(0);
  g_buffer_light_.Set(1);
  screen_size_threshold_.Set(size/6);

  Quad::get().draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

}
