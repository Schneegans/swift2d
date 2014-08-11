////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/PostProcessor.hpp>

#include <swift2d/geometries/Quad.hpp>
#include <swift2d/Swift2D.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PostProcessor::PostProcessor(RenderContext const& ctx, int shading_quality, bool super_sampling)
  : shading_quality_(shading_quality)
  , super_sampling_(super_sampling)
  , post_fx_shader_(R"(
      // vertex shader ---------------------------------------------------------
      @include "fullscreen_quad_vertext_shader"
    )", R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      uniform sampler2D g_buffer_shaded;
      uniform sampler2D glow_buffer_1;
      uniform sampler2D glow_buffer_2;
      uniform sampler2D glow_buffer_3;
      uniform sampler2D glow_buffer_4;
      uniform sampler2D glow_buffer_5;
      uniform sampler2D glow_buffer_6;
      uniform sampler2D glow_buffer_7;
      uniform sampler2D glow_buffer_8;
      uniform sampler2D heat_buffer;
      uniform sampler2D dirt_tex;
      uniform bool      use_heat;

      // varyings
      in vec2 texcoords;

      layout (location = 0) out vec3 fragColor;

      float get_vignette() {

        float coverage = 0.5;
        float softness = 0.5;

        // inigo quilez's great vigneting effect!
        float a = -coverage/softness;
        float b = 1.0/softness;
        vec2 q = texcoords;
        return min(1, a + b*pow( 16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.1 ));
      }

      void main(void) {
        vec3 glow      = texture2D(glow_buffer_1, texcoords).rgb
                       + texture2D(glow_buffer_2, texcoords).rgb
                       + texture2D(glow_buffer_3, texcoords).rgb
                       + texture2D(glow_buffer_4, texcoords).rgb
                       + texture2D(glow_buffer_5, texcoords).rgb
                       + texture2D(glow_buffer_6, texcoords).rgb
                       + texture2D(glow_buffer_7, texcoords).rgb
                       + texture2D(glow_buffer_8, texcoords).rgb;

        vec3 dirt = texture2D(dirt_tex, texcoords).rgb;

        vec2 shifted_texcoords = texcoords;
        if (use_heat) {
          vec2 offset = (texture2D(heat_buffer, texcoords).rg - 0.5) * 0.2;
          shifted_texcoords   += offset;
        }

        fragColor = texture2D(g_buffer_shaded, shifted_texcoords).rgb;
        fragColor = (fragColor + (glow + 0.05) * dirt) * get_vignette();
      }
    )")
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
  , streak_effect_(ctx)
  , ghost_effect_(ctx)
  , heat_effect_(ctx, shading_quality_)
  , dirt_(Swift2D::instance()->get_resource("images", "dirt.jpg")) {

  auto create_texture = [&](
    oglplus::Texture& tex, int width, int height,
    oglplus::enums::PixelDataInternalFormat i_format,
    oglplus::enums::PixelDataFormat         p_format) {

    oglplus::Texture::Active(0);
    ctx.gl.Bound(oglplus::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, oglplus::PixelDataType::Float, nullptr)
      .MinFilter(oglplus::TextureMinFilter::Linear)
      .MagFilter(oglplus::TextureMagFilter::Linear)
      .WrapS(oglplus::TextureWrap::MirroredRepeat)
      .WrapT(oglplus::TextureWrap::MirroredRepeat);
  };

  // threshold members ---------------------------------------------------------
  auto size(ctx.g_buffer_size/6);

  create_texture(
    threshold_buffer_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGB,
    oglplus::PixelDataFormat::RGB
  );

  threshold_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 0, threshold_buffer_, 0
  );
}

////////////////////////////////////////////////////////////////////////////////

void PostProcessor::process(ConstSerializedScenePtr const& scene, RenderContext const& ctx,
                            oglplus::Texture const& final_buffer,
                            oglplus::Texture const& g_buffer_light) {

  if (shading_quality_ > 2 && scene->heat_objects.size() > 0) {
    heat_effect_.process(scene, ctx);
  }

  ctx.gl.Disable(oglplus::Capability::Blend);

  oglplus::Texture::Active(0);
  final_buffer.Bind(oglplus::Texture::Target::_2D);

  oglplus::Texture::Active(1);
  g_buffer_light.Bind(oglplus::Texture::Target::_2D);

  // thresholding
  generate_threshold_buffer(ctx);

  // streaks
  streak_effect_.process(ctx, threshold_buffer_);

  // ghosts
  ghost_effect_.process(ctx, threshold_buffer_);

  ctx.gl.Viewport(ctx.window_size.x(), ctx.window_size.y());
  oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);
  ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);

  post_fx_shader_.use(ctx);
  post_fx_shader_.set_uniform("g_buffer_shaded", 0);

  int start(1);
  start = streak_effect_.bind_buffers(start, ctx);
  start = ghost_effect_.bind_buffers(start, ctx);

  for (int i(1); i<start; ++i) {
    post_fx_shader_.set_uniform("glow_buffer_" + std::to_string(i), i);
  }

  if (shading_quality_ > 2) {
    post_fx_shader_.set_uniform("heat_buffer", start);
    start = heat_effect_.bind_buffers(start, ctx);
    post_fx_shader_.set_uniform("use_heat", 1);
  } else {
    post_fx_shader_.set_uniform("use_heat", 0);
  }

  dirt_.bind(ctx, start);
  post_fx_shader_.set_uniform("dirt_tex", start);

  Quad::instance()->draw(ctx);

  ctx.gl.Enable(oglplus::Capability::Blend);
}

////////////////////////////////////////////////////////////////////////////////

void PostProcessor::generate_threshold_buffer(RenderContext const& ctx) {

  auto size(ctx.g_buffer_size);

  ctx.gl.Viewport(size.x()/6, size.y()/6);

  threshold_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);

  threshold_shader_.use(ctx);
  threshold_shader_.set_uniform("g_buffer_diffuse", 0);
  threshold_shader_.set_uniform("g_buffer_light", 1);
  threshold_shader_.set_uniform("screen_size", size/6);

  Quad::instance()->draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

}
