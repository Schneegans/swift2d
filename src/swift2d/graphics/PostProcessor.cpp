////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
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
  , g_buffer_diffuse_(post_fx_shader_.get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_normal_(post_fx_shader_.get_uniform<int>("g_buffer_normal"))
  , screen_size_(post_fx_shader_.get_uniform<math::vec2i>("screen_size"))
  , l_buffer_(post_fx_shader_.get_uniform<int>("l_buffer"))
  , glow_buffer_(post_fx_shader_.get_uniform<int>("glow_buffers"))
  , heat_buffer_(post_fx_shader_.get_uniform<int>("heat_buffer"))
  , dirt_tex_(post_fx_shader_.get_uniform<int>("dirt_tex"))
  , dirt_opacity_(post_fx_shader_.get_uniform<float>("dirt_opacity"))
  , glow_opacity_(post_fx_shader_.get_uniform<float>("glow_opacity"))
  , use_heat_(post_fx_shader_.get_uniform<int>("use_heat"))
  , gamma_(post_fx_shader_.get_uniform<float>("gamma"))
  , vignette_color_(post_fx_shader_.get_uniform<math::vec4>("vignette_color"))
  , vignette_softness_(post_fx_shader_.get_uniform<float>("vignette_softness"))
  , vignette_coverage_(post_fx_shader_.get_uniform<float>("vignette_coverage"))
  , use_color_grading_(post_fx_shader_.get_uniform<int>("use_color_grading"))
  , color_grading_tex_(post_fx_shader_.get_uniform<int>("color_grading_tex"))
  , color_grading_intensity_(post_fx_shader_.get_uniform<float>("color_grading_intensity"))
  , lens_flare_effect_(ctx)
  , heat_effect_(ctx)
  , dirt_(Paths::get().resource("images", "dirt.png"))
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

    float get_vignette() {
      // inigo quilez's great vigneting effect!
      float a = -vignette_coverage/vignette_softness;
      float b = 1.0/vignette_softness;
      vec2 q = texcoords;
      return clamp(a + b*pow( 16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.1 ), 0, 1) * vignette_color.a + (1-vignette_color.a);
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

    // lbuffer sampling --------------------------------------------------------
    uniform sampler2D l_buffer;

    vec3 get_lighting(vec2 texcoords, vec2 offset) {

      vec2 texcoords_r = (texcoords + offset * 1.0 - 0.5) * 1.0000 + 0.5;
      vec2 texcoords_g = (texcoords + offset * 0.6 - 0.5) * 0.9975 + 0.5;
      vec2 texcoords_b = (texcoords + offset * 0.2 - 0.5) * 0.9950 + 0.5;

      vec2 light_r = texture(l_buffer, texcoords_r).ra;
      vec2 light_g = texture(l_buffer, texcoords_g).ga;
      vec2 light_b = texture(l_buffer, texcoords_b).ba;

      return vec3(
        light_r.x * texture(g_buffer_diffuse, texcoords_r).r + light_r.y * light_r.x,
        light_g.x * texture(g_buffer_diffuse, texcoords_g).g + light_g.y * light_g.x,
        light_b.x * texture(g_buffer_diffuse, texcoords_b).b + light_b.y * light_b.x
      );
    }
  )";

  if (!ctx.dynamic_lighting) {
    f_source << R"(
      vec3 get_color(vec2 texcoords) {
        return get_diffuse(texcoords);
      }

      vec3 apply_glow(vec3 color_in, float vignetting) {
        return color_in;
      }
    )";
  } else if (!ctx.lens_flares) {
    f_source << R"(
      vec3 get_color(vec2 texcoords) {
        return get_lighting(texcoords, vec2(0));
      }

      vec3 apply_glow(vec3 color_in, float vignetting) {
        return color_in;
      }
    )";
  } else {
    f_source << R"(

      uniform sampler2D glow_buffers[1];
      uniform sampler2D heat_buffer;
      uniform sampler2D dirt_tex;
      uniform float     dirt_opacity;
      uniform float     glow_opacity;
      uniform bool      use_heat;

      vec3 get_color(vec2 texcoords) {
        vec2 offset = vec2(0);
        if (use_heat) {
          offset = (texture(heat_buffer, texcoords).rg - 0.5) * 0.2;
        }
        return get_lighting(texcoords, offset);
      }

      vec3 apply_glow(vec3 color_in, float vignetting) {
        vec3 dirt = texture(dirt_tex, texcoords).rgb;

        vec3 glow_0 = texture(glow_buffers[0], texcoords).rgb;
        // vec3 glow_1 = texture(glow_buffers[1], texcoords).rgb;
        // vec3 glow_2 = texture(glow_buffers[2], texcoords).rgb;
        // vec3 glow_3 = texture(glow_buffers[3], texcoords).rgb;
        // vec3 glow_4 = texture(glow_buffers[4], texcoords).rgb;
        // vec3 glow_5 = texture(glow_buffers[5], texcoords).rgb;

        // vec3 bloom = glow_0 * 0.2f
        //            + glow_1 * 0.2f
        //            + glow_2 * 0.2f
        //            + glow_3 * 0.15f
        //            + glow_4 * 0.15f
        //            + glow_5 * 0.10f;

        // vec3 lens_bloom = glow_0 * 0.4f
        //                 + glow_1 * 0.4f
        //                 + glow_2 * 0.4f
        //                 + glow_3 * 0.30f
        //                 + glow_4 * 0.25f
        //                 + glow_5 * 0.15f;

        // bloom.r = mix(bloom.r, lens_bloom.r, (clamp(dirt.r * dirt_opacity, 0.0, 1.0)));
        // bloom.g = mix(bloom.g, lens_bloom.g, (clamp(dirt.g * dirt_opacity, 0.0, 1.0)));
        // bloom.b = mix(bloom.b, lens_bloom.b, (clamp(dirt.b * dirt_opacity, 0.0, 1.0)));

        // return vec3(1) - (vec3(1) - bloom*glow_opacity)*(vec3(1) - color_in*vignetting);

        return color_in + glow_0 * dirt * glow_opacity + max(vec3(0), (1-vignetting) * dirt * (dirt_opacity+0.5) - 0.5);
      }
    )";
  }

  f_source << R"(
    void main(void){
      vec3 result = get_color(texcoords);
      float vignetting = get_vignette();

      result = apply_glow(result, vignetting);
      result = apply_color_grading(result);
      result = mix(vignette_color.rgb, result, vignetting);
      result = apply_gamma(result);

      fragColor = result;
    }
  )";

  post_fx_shader_.set_sources(v_source, f_source.str());
}

////////////////////////////////////////////////////////////////////////////////

void PostProcessor::process(ConstSerializedScenePtr const& scene, RenderContext const& ctx,
                            GBuffer* g_buffer, LBuffer* l_buffer) {

  auto use_postfx_shader = [&]() {
    post_fx_shader_.use();
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

    ogl::Context::Disable(oglplus::Capability::Blend);

    SWIFT_PUSH_GL_RANGE("Composite");

    ogl::Context::Viewport(ctx.window_size.x(), ctx.window_size.y());
    oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);

    use_postfx_shader();

    g_buffer_diffuse_.Set(g_buffer->bind_diffuse(0));

    gamma_.Set(SettingsWrapper::get().Settings->Gamma());
    Quad::get().draw();

    SWIFT_POP_GL_RANGE();

    ogl::Context::Enable(oglplus::Capability::Blend);

  } else {

    if (ctx.heat_effect) {
      SWIFT_PUSH_GL_RANGE("Heat");
      heat_effect_.process(scene, ctx);
      SWIFT_POP_GL_RANGE();
    }

    ogl::Context::Disable(oglplus::Capability::Blend);

    g_buffer->bind_diffuse(0);
    g_buffer->bind_light  (1);
    g_buffer->bind_normal (2);
    l_buffer->bind        (3);

    if (ctx.lens_flares) {
      SWIFT_PUSH_GL_RANGE("Lens Flares");
      lens_flare_effect_.process(ctx);
      SWIFT_POP_GL_RANGE();
    }

    SWIFT_PUSH_GL_RANGE("Composite");
    ogl::Context::Viewport(ctx.window_size.x(), ctx.window_size.y());
    oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);

    use_postfx_shader();

    g_buffer_diffuse_.Set(0);

    l_buffer_.Set(3);
    gamma_.Set(SettingsWrapper::get().Settings->Gamma());

    if (ctx.lens_flares) {
      int start(4);
      lens_flare_effect_.bind_buffer(start, ctx);
      std::vector<int> units = { 4 };
      start += 1;
      // std::vector<int> units = { 4, 5, 6, 7, 8, 9 };
      // start += 6;

      glow_buffer_.Set(units);

      if (ctx.heat_effect) {
        heat_buffer_.Set(start);
        start = heat_effect_.bind_buffers(start, ctx);
        use_heat_.Set(1);
      } else {
        use_heat_.Set(0);
      }

      dirt_tex_.Set(dirt_.bind(ctx, start));
      // dirt_opacity_.Set(scene->dirt_opacity);
      // glow_opacity_.Set(scene->glow_opacity);
      dirt_opacity_.Set(1);
      glow_opacity_.Set(1);
    }

    Quad::get().draw();

    SWIFT_POP_GL_RANGE();

    ogl::Context::Enable(oglplus::Capability::Blend);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
