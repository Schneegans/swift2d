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
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/application/Paths.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PostProcessor::PostProcessor(RenderContext const& ctx)
  : post_fx_shader_(R"(
      // vertex shader ---------------------------------------------------------
      @include "fullscreen_quad_vertext_shader"
    )",
    ctx.shading_quality <= 1 ?
    R"(
      // fragment shader -----------------------------------------------------
      @include "version"

      in vec2 texcoords;
      uniform sampler2D g_buffer_shaded;
      uniform float gamma;

      @include "get_vignette"

      layout (location = 0) out vec3 fragColor;

      void main(void){
        fragColor = mix(vignette_color.rgb, texture2D(g_buffer_shaded, texcoords).rgb, get_vignette());
        fragColor = pow(fragColor, 1.0/vec3(gamma));
      }
    )" : R"(
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
      uniform float     dirt_opacity;
      uniform bool      use_heat;
      uniform float     gamma;

      // varyings
      in vec2 texcoords;

      layout (location = 0) out vec3 fragColor;

      @include "get_vignette"

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
        fragColor = mix(vignette_color.rgb, (fragColor + (glow + 0.1) * dirt * dirt_opacity), get_vignette());
        fragColor = pow(fragColor, 1.0/vec3(gamma));
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
  , g_buffer_shaded_(post_fx_shader_.get_uniform<int>("g_buffer_shaded"))
  , glow_buffer_1_(post_fx_shader_.get_uniform<int>("glow_buffer_1"))
  , glow_buffer_2_(post_fx_shader_.get_uniform<int>("glow_buffer_2"))
  , glow_buffer_3_(post_fx_shader_.get_uniform<int>("glow_buffer_3"))
  , glow_buffer_4_(post_fx_shader_.get_uniform<int>("glow_buffer_4"))
  , glow_buffer_5_(post_fx_shader_.get_uniform<int>("glow_buffer_5"))
  , glow_buffer_6_(post_fx_shader_.get_uniform<int>("glow_buffer_6"))
  , glow_buffer_7_(post_fx_shader_.get_uniform<int>("glow_buffer_7"))
  , glow_buffer_8_(post_fx_shader_.get_uniform<int>("glow_buffer_8"))
  , heat_buffer_(post_fx_shader_.get_uniform<int>("heat_buffer"))
  , dirt_tex_(post_fx_shader_.get_uniform<int>("dirt_tex"))
  , dirt_opacity_(post_fx_shader_.get_uniform<float>("dirt_opacity"))
  , use_heat_(post_fx_shader_.get_uniform<int>("use_heat"))
  , gamma_(post_fx_shader_.get_uniform<float>("gamma"))
  , vignette_color_(post_fx_shader_.get_uniform<math::vec4>("vignette_color"))
  , vignette_softness_(post_fx_shader_.get_uniform<float>("vignette_softness"))
  , vignette_coverage_(post_fx_shader_.get_uniform<float>("vignette_coverage"))
  , screen_size_(threshold_shader_.get_uniform<math::vec2i>("screen_size"))
  , g_buffer_diffuse_(threshold_shader_.get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_light_(threshold_shader_.get_uniform<int>("g_buffer_light"))
  , streak_effect_(ctx)
  , ghost_effect_(ctx)
  , heat_effect_(ctx)
  , dirt_(Paths::get().resource("images", "dirt.jpg")) {

  auto create_texture = [&](
    oglplus::Texture& tex, int width, int height,
    oglplus::enums::PixelDataInternalFormat i_format,
    oglplus::enums::PixelDataFormat         p_format) {

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
                            GBuffer* g_buffer) {

  post_fx_shader_.use(ctx);
  vignette_softness_.Set(scene->vignette_softness);
  vignette_coverage_.Set(scene->vignette_coverage);
  vignette_color_.Set(scene->vignette_color);

  if (ctx.shading_quality <= 1) {

    ctx.gl.Disable(oglplus::Capability::Blend);

    ctx.gl.Viewport(ctx.window_size.x(), ctx.window_size.y());
    oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);

    if (ctx.shading_quality == 0) {
      g_buffer->bind_diffuse(1);
    } else {
      g_buffer->bind_final(1);
    }
    post_fx_shader_.set_uniform("g_buffer_shaded", 1);
    gamma_.Set(SettingsWrapper::get().Settings->Gamma());
    Quad::get().draw(ctx);

    ctx.gl.Enable(oglplus::Capability::Blend);

  } else {

    if (ctx.shading_quality > 2 && scene->heat_objects.size() > 0) {
      heat_effect_.process(scene, ctx);
    }

    ctx.gl.Disable(oglplus::Capability::Blend);

    g_buffer->bind_final(0);
    g_buffer->bind_light(1);

    // thresholding
    generate_threshold_buffer(ctx);

    // streaks
    streak_effect_.process(ctx, threshold_buffer_);

    // ghosts
    ghost_effect_.process(ctx, threshold_buffer_);

    ctx.gl.Viewport(ctx.window_size.x(), ctx.window_size.y());
    oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);

    post_fx_shader_.use(ctx);
    g_buffer_shaded_.Set(0);

    int start(1);
    start = streak_effect_.bind_buffers(start, ctx);
    start = ghost_effect_.bind_buffers(start, ctx);

    glow_buffer_1_.Set(1);
    glow_buffer_2_.Set(2);
    glow_buffer_3_.Set(3);
    glow_buffer_4_.Set(4);
    glow_buffer_5_.Set(5);
    glow_buffer_6_.Set(6);
    glow_buffer_7_.Set(7);
    glow_buffer_8_.Set(8);

    if (ctx.shading_quality > 2) {
      heat_buffer_.Set(start);
      start = heat_effect_.bind_buffers(start, ctx);
      use_heat_.Set(1);
    } else {
      use_heat_.Set(0);
    }

    gamma_.Set(SettingsWrapper::get().Settings->Gamma());

    dirt_.bind(ctx, start);
    dirt_tex_.Set(start);
    dirt_opacity_.Set(scene->dirt_opacity);

    Quad::get().draw(ctx);

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
  g_buffer_diffuse_.Set(0);
  g_buffer_light_.Set(1);
  screen_size_.Set(size/6);

  Quad::get().draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

}
