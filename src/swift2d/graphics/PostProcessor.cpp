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
#include <swift2d/components/DrawableComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PostProcessor::PostProcessor(RenderContext const& ctx, int shading_quality)
  : shading_quality_(shading_quality)
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
      uniform sampler2D heat_buffer;
      uniform bool      use_heat;
      uniform ivec2     screen_size;

      layout (location = 0) out vec3 fragColor;

      void main(void) {
        vec2 texcoords = gl_FragCoord.xy/screen_size;
        vec3 glow      = texture2D(glow_buffer_1, texcoords).rgb
                       + texture2D(glow_buffer_2, texcoords).rgb
                       + texture2D(glow_buffer_3, texcoords).rgb
                       + texture2D(glow_buffer_4, texcoords).rgb
                       + texture2D(glow_buffer_5, texcoords).rgb
                       + texture2D(glow_buffer_6, texcoords).rgb;

        if (use_heat) {
          vec2 offset = (texture2D(heat_buffer, texcoords).rg - 0.5) * 0.2;
          texcoords   += offset;
        }

        fragColor    = texture2D(g_buffer_shaded, texcoords).rgb;
        // fragColor.b += glow;

        fragColor = fragColor + glow*0.75;
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

        fragColor = glow * color / 16.0;
      }
    )")
  , streak_effect_(ctx) {

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
  auto size(ctx.size/8);

  create_texture(
    threshold_buffer_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGB,
    oglplus::PixelDataFormat::RGB
  );

  threshold_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 0, threshold_buffer_, 0
  );

  if (shading_quality_ > 2) {

    // heat members ------------------------------------------------------------
    create_texture(
      heat_buffer_, ctx.size.x()/8, ctx.size.y()/8,
      oglplus::PixelDataInternalFormat::RG,
      oglplus::PixelDataFormat::RG
    );

    heat_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 0, heat_buffer_, 0
    );

    GLfloat clear[2] = {0.5f, 0.5f};
    ctx.gl.ClearColorBuffer(0, clear);
  }
}

////////////////////////////////////////////////////////////////////////////////

void PostProcessor::draw_heat_objects(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  if (shading_quality_ > 2) {
    ctx.gl.BlendFunc(
      oglplus::BlendFunction::SrcAlpha,
      oglplus::BlendFunction::OneMinusSrcAlpha
    );

    ctx.gl.Viewport(ctx.size.x()/8, ctx.size.y()/8);

    heat_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
    ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);

    GLfloat clear[2] = {0.5f, 0.5f};
    ctx.gl.ClearColorBuffer(0, clear);

    for (auto& object: scene->heat_objects) {
      object.second->draw(ctx);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void PostProcessor::process(RenderContext const& ctx) {
  ctx.gl.Disable(oglplus::Capability::Blend);

  // thresholding
  generate_threshold_buffer(ctx);

  // streaks
  streak_effect_.process(ctx, threshold_buffer_);
  streak_effect_.bind_buffers(ctx);

  ctx.gl.Viewport(ctx.size.x(), ctx.size.y());

  oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);
  ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);

  post_fx_shader_.use(ctx);

  if (shading_quality_ > 2) {
    oglplus::Texture::Active(1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), heat_buffer_);
    post_fx_shader_.set_uniform("heat_buffer", 1);
    post_fx_shader_.set_uniform("use_heat", 1);
  } else {
    post_fx_shader_.set_uniform("use_heat", 0);
  }

  post_fx_shader_.set_uniform("g_buffer_shaded", 0);
  post_fx_shader_.set_uniform("glow_buffer_1", 2);
  post_fx_shader_.set_uniform("glow_buffer_2", 3);
  post_fx_shader_.set_uniform("glow_buffer_3", 4);
  post_fx_shader_.set_uniform("glow_buffer_4", 5);
  post_fx_shader_.set_uniform("glow_buffer_5", 6);
  post_fx_shader_.set_uniform("glow_buffer_6", 7);
  post_fx_shader_.set_uniform("screen_size", ctx.size);

  Quad::instance()->draw(ctx);

  ctx.gl.Enable(oglplus::Capability::Blend);
}

////////////////////////////////////////////////////////////////////////////////

void PostProcessor::generate_threshold_buffer(RenderContext const& ctx) {

  ctx.gl.Viewport(ctx.size.x()/8, ctx.size.y()/8);

  threshold_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);

  threshold_shader_.use(ctx);
  threshold_shader_.set_uniform("g_buffer_diffuse", 0);
  threshold_shader_.set_uniform("g_buffer_light", 2);
  threshold_shader_.set_uniform("screen_size", ctx.size/8);

  Quad::instance()->draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

}
