////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/StreakEffect.hpp>

#include <swift2d/geometries/Quad.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

StreakEffect::StreakEffect(RenderContext const& ctx)
: streak_shader_(R"(
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
  , step_(streak_shader_.get_uniform<math::vec2>("step"))
  , input_tex_(streak_shader_.get_uniform<int>("input_tex"))
  , color1_(streak_shader_.get_uniform<math::vec3>("color1"))
  , color2_(streak_shader_.get_uniform<math::vec3>("color2"))
  , color3_(streak_shader_.get_uniform<math::vec3>("color3"))
  , color4_(streak_shader_.get_uniform<math::vec3>("color4"))  {

  auto create_texture = [&](
    ogl::Texture& tex, int width, int height,
    ogl::enums::PixelDataInternalFormat i_format,
    ogl::enums::PixelDataFormat         p_format) {

    ctx.gl.Bound(ogl::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, ogl::PixelDataType::Float, nullptr)
      .MinFilter(ogl::TextureMinFilter::Linear)
      .MagFilter(ogl::TextureMagFilter::Linear)
      .WrapS(ogl::TextureWrap::ClampToBorder)
      .WrapT(ogl::TextureWrap::ClampToBorder);
  };

  auto size(ctx.g_buffer_size/6);

  create_texture(
    streak_buffer_tmp_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_1_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_2_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_3_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_4_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_5_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_6_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
    ogl::PixelDataFormat::RGB
  );

  streak_fbo_.Bind(ogl::Framebuffer::Target::Draw);
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 0, streak_buffer_tmp_, 0
  );
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 1, streak_buffer_1_, 0
  );
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 2, streak_buffer_2_, 0
  );
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 3, streak_buffer_3_, 0
  );
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 4, streak_buffer_4_, 0
  );
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 5, streak_buffer_5_, 0
  );
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 6, streak_buffer_6_, 0
  );

  math::vec3 c1(1.0, 1.0, 1.0);
  math::vec3 c2(0.3, 0.8, 1.0);
  math::vec3 c3(0.1, 0.4, 1.0);
  math::vec3 c4(0.0, 0.0, 1.0);

  math::vec3 c5(1.0, 0.8, 0.9);
  math::vec3 c6(0.5, 0.4, 0.7);
  math::vec3 c7(0.2, 0.2, 0.6);

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
}

////////////////////////////////////////////////////////////////////////////////

void StreakEffect::process(RenderContext const& ctx, ogl::Texture const& threshold_buffer_) {

  ctx.gl.Viewport(ctx.g_buffer_size.x()/6, ctx.g_buffer_size.y()/6);

  streak_fbo_.Bind(ogl::Framebuffer::Target::Draw);
  const float DEC = 0.96;
  streak_shader_.use(ctx);
  input_tex_.Set(4);
  ogl::Texture::Active(4);

  auto pass = [&](math::vec2 const& step_dir, ogl::Texture const& input,
                  ogl::FramebufferColorAttachment output,
                  std::vector<math::vec3>& colors_in) {

    ctx.gl.DrawBuffer(output);
    ctx.gl.Bind(ose::_2D(), input);

    color1_.Set(colors_in[0]);
    color2_.Set(colors_in[1]);
    color3_.Set(colors_in[2]);
    color4_.Set(colors_in[3]);

    step_.Set(step_dir);

    Quad::instance()->draw(ctx);
  };

  math::vec2 step_dir(0.9*4.0/ctx.g_buffer_size.x(), 0.0);
  pass(step_dir*35, threshold_buffer_,  ogl::FramebufferColorAttachment::_0, streak_colors_1_);
  pass(step_dir*12, streak_buffer_tmp_, ogl::FramebufferColorAttachment::_1, streak_colors_2_);
  pass(step_dir*5,  streak_buffer_1_,   ogl::FramebufferColorAttachment::_0, streak_colors_3_);
  pass(step_dir*2,  streak_buffer_tmp_, ogl::FramebufferColorAttachment::_1, streak_colors_3_);

  step_dir = math::vec2(-0.9*4.0/ctx.g_buffer_size.x(), 0.0);
  pass(step_dir*35, threshold_buffer_,  ogl::FramebufferColorAttachment::_0, streak_colors_1_);
  pass(step_dir*12, streak_buffer_tmp_, ogl::FramebufferColorAttachment::_2, streak_colors_2_);
  pass(step_dir*5,  streak_buffer_2_,   ogl::FramebufferColorAttachment::_0, streak_colors_3_);
  pass(step_dir*2,  streak_buffer_tmp_, ogl::FramebufferColorAttachment::_2, streak_colors_3_);

  step_dir = math::vec2(-0.7*4.0/ctx.g_buffer_size.x(), -0.9*4.0/ctx.g_buffer_size.y());
  pass(step_dir*5, threshold_buffer_,  ogl::FramebufferColorAttachment::_0, streak_colors_4_);
  pass(step_dir*2,  streak_buffer_tmp_, ogl::FramebufferColorAttachment::_3, streak_colors_3_);

  step_dir = math::vec2(0.7*4.0/ctx.g_buffer_size.x(), -0.9*4.0/ctx.g_buffer_size.y());
  pass(step_dir*5, threshold_buffer_,  ogl::FramebufferColorAttachment::_0, streak_colors_4_);
  pass(step_dir*2,  streak_buffer_tmp_, ogl::FramebufferColorAttachment::_4, streak_colors_3_);

  step_dir = math::vec2(-0.7*4.0/ctx.g_buffer_size.x(), 0.9*4.0/ctx.g_buffer_size.y());
  pass(step_dir*5, threshold_buffer_,  ogl::FramebufferColorAttachment::_0, streak_colors_4_);
  pass(step_dir*2,  streak_buffer_tmp_, ogl::FramebufferColorAttachment::_5, streak_colors_3_);

  step_dir = math::vec2(0.7*4.0/ctx.g_buffer_size.x(), 0.9*4.0/ctx.g_buffer_size.y());
  pass(step_dir*5, threshold_buffer_,  ogl::FramebufferColorAttachment::_0, streak_colors_4_);
  pass(step_dir*2,  streak_buffer_tmp_, ogl::FramebufferColorAttachment::_6, streak_colors_3_);
}

////////////////////////////////////////////////////////////////////////////////

int StreakEffect::bind_buffers(int start, RenderContext const& ctx) {
  ogl::Texture::Active(start + 0);
  ctx.gl.Bind(ose::_2D(), streak_buffer_1_);

  ogl::Texture::Active(start + 1);
  ctx.gl.Bind(ose::_2D(), streak_buffer_2_);

  ogl::Texture::Active(start + 2);
  ctx.gl.Bind(ose::_2D(), streak_buffer_3_);

  ogl::Texture::Active(start + 3);
  ctx.gl.Bind(ose::_2D(), streak_buffer_4_);

  ogl::Texture::Active(start + 4);
  ctx.gl.Bind(ose::_2D(), streak_buffer_5_);

  ogl::Texture::Active(start + 5);
  ctx.gl.Bind(ose::_2D(), streak_buffer_6_);

  return start + 6;
}

////////////////////////////////////////////////////////////////////////////////

}
