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
  , streak_shader_(R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2 position;

      uniform vec2 step;

      out vec2 texcoord1;
      out vec2 texcoord2;
      out vec2 texcoord3;
      out vec2 texcoord4;

      void main(void){
        texcoord1 = vec2(position.x + 1.0, 1.0 + position.y) * 0.5;
        texcoord2 = texcoord1 + step;
        texcoord3 = texcoord1 + step * 2.0;
        texcoord4 = texcoord1 + step * 3.0;
        gl_Position = vec4(position, 0.0, 1.0);
      }
    )", R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      uniform sampler2D input_tex;
      uniform vec3 color1;
      uniform vec3 color2;
      uniform vec3 color3;
      uniform vec3 color4;

      in vec2 texcoord1;
      in vec2 texcoord2;
      in vec2 texcoord3;
      in vec2 texcoord4;

      layout (location = 0) out vec3 fragColor;

      void main(void) {
        fragColor = texture2D(input_tex, texcoord1).rgb * color1
                  + texture2D(input_tex, texcoord2).rgb * color2
                  + texture2D(input_tex, texcoord3).rgb * color3
                  + texture2D(input_tex, texcoord4).rgb * color4;
      }
    )")
  // , glow_shader_(R"(
  //     // vertex shader -------------------------------------------------------
  //     @include "version"

  //     layout(location=0) in vec2 position;

  //     uniform ivec2 screen_size;
  //     uniform float flare_length;

  //     out vec2 blur_texcoords[14];

  //     void main(void){
  //       gl_Position     = vec4(position, 0.0, 1.0);
  //       vec2 tex_coords = vec2(position.x + 1.0, position.y + 1.0) * 0.5;
  //       vec2 scale = vec2(1, 0) * flare_length / screen_size;

  //       blur_texcoords[ 0] = tex_coords + vec2(-1.000) * scale;
  //       blur_texcoords[ 1] = tex_coords + vec2(-0.857) * scale;
  //       blur_texcoords[ 2] = tex_coords + vec2(-0.714) * scale;
  //       blur_texcoords[ 3] = tex_coords + vec2(-0.571) * scale;
  //       blur_texcoords[ 4] = tex_coords + vec2(-0.428) * scale;
  //       blur_texcoords[ 5] = tex_coords + vec2(-0.285) * scale;
  //       blur_texcoords[ 6] = tex_coords + vec2(-0.143) * scale;
  //       blur_texcoords[ 7] = tex_coords + vec2( 0.143) * scale;
  //       blur_texcoords[ 8] = tex_coords + vec2( 0.285) * scale;
  //       blur_texcoords[ 9] = tex_coords + vec2( 0.428) * scale;
  //       blur_texcoords[10] = tex_coords + vec2( 0.571) * scale;
  //       blur_texcoords[11] = tex_coords + vec2( 0.714) * scale;
  //       blur_texcoords[12] = tex_coords + vec2( 0.857) * scale;
  //       blur_texcoords[13] = tex_coords + vec2( 1.000) * scale;
  //     }
  //   )", R"(
  //     // fragment shader -----------------------------------------------------
  //     @include "version"

  //     uniform sampler2D glow_buffer;
  //     uniform ivec2     screen_size;
  //     uniform float     flare_length;

  //     in vec2 blur_texcoords[14];

  //     layout (location = 0) out vec4 fragColor;

  //     void main(void) {

  //       vec2 texcoords = gl_FragCoord.xy/screen_size;

  //       float col = 0.0;

  //       col += texture2D(glow_buffer, blur_texcoords[ 0]).r*0.0044299121055113265;
  //       col += texture2D(glow_buffer, blur_texcoords[ 1]).r*0.00895781211794;
  //       col += texture2D(glow_buffer, blur_texcoords[ 2]).r*0.0215963866053;
  //       col += texture2D(glow_buffer, blur_texcoords[ 3]).r*0.0443683338718;
  //       col += texture2D(glow_buffer, blur_texcoords[ 4]).r*0.0776744219933;
  //       col += texture2D(glow_buffer, blur_texcoords[ 5]).r*0.115876621105;
  //       col += texture2D(glow_buffer, blur_texcoords[ 6]).r*0.147308056121;
  //       col += texture2D(glow_buffer, texcoords         ).r*0.159576912161;
  //       col += texture2D(glow_buffer, blur_texcoords[ 7]).r*0.147308056121;
  //       col += texture2D(glow_buffer, blur_texcoords[ 8]).r*0.115876621105;
  //       col += texture2D(glow_buffer, blur_texcoords[ 9]).r*0.0776744219933;
  //       col += texture2D(glow_buffer, blur_texcoords[10]).r*0.0443683338718;
  //       col += texture2D(glow_buffer, blur_texcoords[11]).r*0.0215963866053;
  //       col += texture2D(glow_buffer, blur_texcoords[12]).r*0.00895781211794;
  //       col += texture2D(glow_buffer, blur_texcoords[13]).r*0.0044299121055113265;

  //       fragColor = vec4(col, 0.0, 0.0, 1.0);
  //     }
  //   )") {
    {

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

  // streak members ------------------------------------------------------------
  create_texture(
    streak_buffer_tmp_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGB,
    oglplus::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_1_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGB,
    oglplus::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_2_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGB,
    oglplus::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_3_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGB,
    oglplus::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_4_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGB,
    oglplus::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_5_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGB,
    oglplus::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_6_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGB,
    oglplus::PixelDataFormat::RGB
  );

  streak_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 0, streak_buffer_tmp_, 0
  );
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 1, streak_buffer_1_, 0
  );
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 2, streak_buffer_2_, 0
  );
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 3, streak_buffer_3_, 0
  );
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 4, streak_buffer_4_, 0
  );
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 5, streak_buffer_5_, 0
  );
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 6, streak_buffer_6_, 0
  );

  math::vec3 c1(1.0, 1.0, 1.0);
  math::vec3 c2(0.3, 0.8, 1.0);
  math::vec3 c3(0.1, 0.4, 1.0);
  math::vec3 c4(0.0, 0.0, 1.0);

  math::vec3 c5(1.0, 0.8, 0.9);
  math::vec3 c6(1.0, 0.4, 0.7);
  math::vec3 c7(1.0, 0.2, 0.6);

  streak_colors_1_.push_back(c1*0.9);
  streak_colors_1_.push_back(c2*0.7);
  streak_colors_1_.push_back(c3*0.5);
  streak_colors_1_.push_back(c4*0.3);

  streak_colors_2_.push_back(c2*0.25);
  streak_colors_2_.push_back(c2*0.25);
  streak_colors_2_.push_back(c3*0.25);
  streak_colors_2_.push_back(c3*0.25);

  streak_colors_3_.push_back(c1*0.25);
  streak_colors_3_.push_back(c1*0.25);
  streak_colors_3_.push_back(c1*0.25);
  streak_colors_3_.push_back(c1*0.25);

  streak_colors_4_.push_back(c1*0.5);
  streak_colors_4_.push_back(c5*0.35);
  streak_colors_4_.push_back(c6*0.2);
  streak_colors_4_.push_back(c7*0.1);


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
  generate_streak_buffers(ctx);

  // // glow blurring
  // glow_shader_.use(ctx);
  // glow_shader_.set_uniform("screen_size", ctx.size/4);
  // oglplus::Texture::Active(1);
  // glow_shader_.set_uniform("glow_buffer", 1);

  // // pass 1
  // ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_1);
  // ctx.gl.Bind(oglplus::smart_enums::_2D(), glow_ping_);
  // glow_shader_.set_uniform("flare_length", 20.f);
  // Quad::instance()->draw(ctx);

  // // pass 2
  // ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);
  // ctx.gl.Bind(oglplus::smart_enums::_2D(), glow_pong_);
  // glow_shader_.set_uniform("flare_length", 100.f);
  // Quad::instance()->draw(ctx);

  // // pass 3
  // ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_1);
  // ctx.gl.Bind(oglplus::smart_enums::_2D(), glow_ping_);
  // glow_shader_.set_uniform("flare_length", 400.f);
  // Quad::instance()->draw(ctx);

  // // pass 4
  // ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);
  // ctx.gl.Bind(oglplus::smart_enums::_2D(), glow_pong_);
  // glow_shader_.set_uniform("flare_length", 0.5f);
  // Quad::instance()->draw(ctx);

  ctx.gl.Viewport(ctx.size.x(), ctx.size.y());

  oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);
  ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);

  oglplus::Texture::Active(2);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), streak_buffer_1_);

  oglplus::Texture::Active(3);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), streak_buffer_2_);

  oglplus::Texture::Active(4);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), streak_buffer_3_);

  oglplus::Texture::Active(5);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), streak_buffer_4_);

  oglplus::Texture::Active(6);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), streak_buffer_5_);

  oglplus::Texture::Active(7);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), streak_buffer_6_);

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

void PostProcessor::generate_streak_buffers(RenderContext const& ctx) {

  streak_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  const float DEC = 0.96;
  streak_shader_.use(ctx);
  streak_shader_.set_uniform("input_tex", 4);
  oglplus::Texture::Active(4);

  auto pass = [&](math::vec2 const& step, oglplus::Texture& input,
                  oglplus::FramebufferColorAttachment output,
                  std::vector<math::vec3>& colors_in) {

    ctx.gl.DrawBuffer(output);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), input);

    for (int s(0); s<4; ++s) {
      streak_shader_.set_uniform("color" + std::to_string(s+1), colors_in[s]);
    }

    streak_shader_.set_uniform("step", step);

    Quad::instance()->draw(ctx);
  };

  math::vec2 step(0.9*4.0/ctx.size.x(), 0.0);
  pass(step*35, threshold_buffer_,  oglplus::FramebufferColorAttachment::_0, streak_colors_1_);
  pass(step*12, streak_buffer_tmp_, oglplus::FramebufferColorAttachment::_1, streak_colors_2_);
  pass(step*5,  streak_buffer_1_,   oglplus::FramebufferColorAttachment::_0, streak_colors_3_);
  pass(step*2,  streak_buffer_tmp_, oglplus::FramebufferColorAttachment::_1, streak_colors_3_);

  step = math::vec2(-0.9*4.0/ctx.size.x(), 0.0);
  pass(step*35, threshold_buffer_,  oglplus::FramebufferColorAttachment::_0, streak_colors_1_);
  pass(step*12, streak_buffer_tmp_, oglplus::FramebufferColorAttachment::_2, streak_colors_2_);
  pass(step*5,  streak_buffer_2_,   oglplus::FramebufferColorAttachment::_0, streak_colors_3_);
  pass(step*2,  streak_buffer_tmp_, oglplus::FramebufferColorAttachment::_2, streak_colors_3_);

  step = math::vec2(-0.7*4.0/ctx.size.x(), -0.9*4.0/ctx.size.y());
  pass(step*10, threshold_buffer_,  oglplus::FramebufferColorAttachment::_0, streak_colors_4_);
  pass(step*2,  streak_buffer_tmp_, oglplus::FramebufferColorAttachment::_3, streak_colors_3_);

  step = math::vec2(0.7*4.0/ctx.size.x(), -0.9*4.0/ctx.size.y());
  pass(step*10, threshold_buffer_,  oglplus::FramebufferColorAttachment::_0, streak_colors_4_);
  pass(step*2,  streak_buffer_tmp_, oglplus::FramebufferColorAttachment::_4, streak_colors_3_);

  step = math::vec2(-0.7*4.0/ctx.size.x(), 0.9*4.0/ctx.size.y());
  pass(step*10, threshold_buffer_,  oglplus::FramebufferColorAttachment::_0, streak_colors_4_);
  pass(step*2,  streak_buffer_tmp_, oglplus::FramebufferColorAttachment::_5, streak_colors_3_);

  step = math::vec2(0.7*4.0/ctx.size.x(), 0.9*4.0/ctx.size.y());
  pass(step*10, threshold_buffer_,  oglplus::FramebufferColorAttachment::_0, streak_colors_4_);
  pass(step*2,  streak_buffer_tmp_, oglplus::FramebufferColorAttachment::_6, streak_colors_3_);
}

////////////////////////////////////////////////////////////////////////////////

}
