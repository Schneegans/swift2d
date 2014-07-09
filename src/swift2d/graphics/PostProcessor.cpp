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

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PostProcessor::PostProcessor(RenderContext const& ctx)
  : post_fx_shader_(R"(
      // vertex shader -------------------------------------------------------
      @include "fullscreen_quad_vertext_shader"
    )", R"(
      // fragment shader -----------------------------------------------------
      @include "version"

      uniform sampler2D g_buffer_shaded;
      uniform sampler2D g_glow;
      uniform ivec2     screen_size;

      layout (location = 0) out vec4 fragColor;

      void main(void) {
        vec3  diffuse  = texture2D(g_buffer_shaded, gl_FragCoord.xy/screen_size).rgb;
        float glow     = texture2D(g_glow, gl_FragCoord.xy/screen_size).r;

        fragColor      = vec4(diffuse + vec3(0, 0, glow), 1.0);
      }
    )")
  , glow_threshold_shader_(R"(
      // vertex shader -------------------------------------------------------
      @include "fullscreen_quad_vertext_shader"
    )", R"(
      // fragment shader -----------------------------------------------------
      @include "version"

      @include "gbuffer_input"

      layout (location = 0) out vec4 fragColor;

      void main(void) {
        vec3 c = get_diffuse();
        fragColor = vec4(get_glow() * max(max(c.r, c.g), c.b), 0.0, 0.0, 1.0);
      }
    )")
  , glow_shader_(R"(
      // vertex shader -------------------------------------------------------
      @include "version"

      layout(location=0) in vec2 position;

      uniform ivec2 screen_size;
      uniform float flare_length;

      out vec2 blur_texcoords[14];

      void main(void){
        gl_Position     = vec4(position, 0.0, 1.0);
        vec2 tex_coords = vec2(position.x + 1.0, position.y + 1.0) * 0.5;
        vec2 scale = vec2(1, 0) * flare_length / screen_size;

        blur_texcoords[ 0] = tex_coords + vec2(-1.000) * scale;
        blur_texcoords[ 1] = tex_coords + vec2(-0.857) * scale;
        blur_texcoords[ 2] = tex_coords + vec2(-0.714) * scale;
        blur_texcoords[ 3] = tex_coords + vec2(-0.571) * scale;
        blur_texcoords[ 4] = tex_coords + vec2(-0.428) * scale;
        blur_texcoords[ 5] = tex_coords + vec2(-0.285) * scale;
        blur_texcoords[ 6] = tex_coords + vec2(-0.143) * scale;
        blur_texcoords[ 7] = tex_coords + vec2( 0.143) * scale;
        blur_texcoords[ 8] = tex_coords + vec2( 0.285) * scale;
        blur_texcoords[ 9] = tex_coords + vec2( 0.428) * scale;
        blur_texcoords[10] = tex_coords + vec2( 0.571) * scale;
        blur_texcoords[11] = tex_coords + vec2( 0.714) * scale;
        blur_texcoords[12] = tex_coords + vec2( 0.857) * scale;
        blur_texcoords[13] = tex_coords + vec2( 1.000) * scale;
      }
    )", R"(
      // fragment shader -----------------------------------------------------
      @include "version"

      uniform sampler2D g_glow;
      uniform ivec2     screen_size;
      uniform float     flare_length;

      in vec2 blur_texcoords[14];

      layout (location = 0) out vec4 fragColor;

      void main(void) {

        vec2 texcoords = gl_FragCoord.xy/screen_size;

        float col = 0.0;

        col += texture2D(g_glow, blur_texcoords[ 0]).r*0.0044299121055113265;
        col += texture2D(g_glow, blur_texcoords[ 1]).r*0.00895781211794;
        col += texture2D(g_glow, blur_texcoords[ 2]).r*0.0215963866053;
        col += texture2D(g_glow, blur_texcoords[ 3]).r*0.0443683338718;
        col += texture2D(g_glow, blur_texcoords[ 4]).r*0.0776744219933;
        col += texture2D(g_glow, blur_texcoords[ 5]).r*0.115876621105;
        col += texture2D(g_glow, blur_texcoords[ 6]).r*0.147308056121;
        col += texture2D(g_glow, texcoords         ).r*0.159576912161;
        col += texture2D(g_glow, blur_texcoords[ 7]).r*0.147308056121;
        col += texture2D(g_glow, blur_texcoords[ 8]).r*0.115876621105;
        col += texture2D(g_glow, blur_texcoords[ 9]).r*0.0776744219933;
        col += texture2D(g_glow, blur_texcoords[10]).r*0.0443683338718;
        col += texture2D(g_glow, blur_texcoords[11]).r*0.0215963866053;
        col += texture2D(g_glow, blur_texcoords[12]).r*0.00895781211794;
        col += texture2D(g_glow, blur_texcoords[13]).r*0.0044299121055113265;

        fragColor = vec4(col, 0.0, 0.0, 1.0);
      }
    )")
  , glow_fbo_()
  , glow_ping_()
  , glow_pong_() {

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

  create_texture(
    glow_ping_, ctx.size.x()/2, ctx.size.y()/2,
    oglplus::PixelDataInternalFormat::Red,
    oglplus::PixelDataFormat::Red
  );

  create_texture(
    glow_pong_, ctx.size.x()/2, ctx.size.y()/2,
    oglplus::PixelDataInternalFormat::Red,
    oglplus::PixelDataFormat::Red
  );

  // create framebuffer object ---------------------------------------------
  glow_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 0, glow_ping_, 0
  );
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 1, glow_pong_, 0
  );
}

////////////////////////////////////////////////////////////////////////////////

void PostProcessor::process(RenderContext const& ctx) {
  ctx.gl.Disable(oglplus::Capability::Blend);

  // glow thresholding
  glow_fbo_.Bind(oglplus::Framebuffer::Target::Draw);

  ctx.gl.Viewport(ctx.size.x()/2, ctx.size.y()/2);

  ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);

  glow_threshold_shader_.use(ctx);
  // glow_threshold_shader_.set_uniform("g_buffer_diffuse", 0);
  glow_threshold_shader_.set_uniform("g_buffer_light", 2);
  glow_threshold_shader_.set_uniform("screen_size", ctx.size/2);

  Quad::instance()->draw(ctx);

  // glow blurring
  glow_shader_.use(ctx);
  glow_shader_.set_uniform("screen_size", ctx.size/2);
  oglplus::Texture::Active(1);
  glow_shader_.set_uniform("g_glow", 1);

  // pass 1
  ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_1);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), glow_ping_);
  glow_shader_.set_uniform("flare_length", 20.f);
  Quad::instance()->draw(ctx);

  // pass 2
  ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), glow_pong_);
  glow_shader_.set_uniform("flare_length", 100.f);
  Quad::instance()->draw(ctx);

  // pass 3
  ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_1);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), glow_ping_);
  glow_shader_.set_uniform("flare_length", 400.f);
  Quad::instance()->draw(ctx);

  // // pass 4
  // ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);
  // ctx.gl.Bind(oglplus::smart_enums::_2D(), glow_pong_);
  // glow_shader_.set_uniform("flare_length", 0.5f);
  // Quad::instance()->draw(ctx);

  ctx.gl.Viewport(ctx.size.x(), ctx.size.y());

  oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);
  ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);

  oglplus::Texture::Active(1);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), glow_pong_);

  post_fx_shader_.use(ctx);
  post_fx_shader_.set_uniform("g_buffer_shaded", 0);
  post_fx_shader_.set_uniform("g_glow", 1);
  post_fx_shader_.set_uniform("screen_size", ctx.size);

  Quad::instance()->draw(ctx);

  ctx.gl.Enable(oglplus::Capability::Blend);
}

////////////////////////////////////////////////////////////////////////////////

}
