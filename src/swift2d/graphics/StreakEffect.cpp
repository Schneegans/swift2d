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
#include <swift2d/graphics/StreakEffect.hpp>

#include <swift2d/geometries/Quad.hpp>

#define GBUFFER_FRACTION 4

namespace swift {

////////////////////////////////////////////////////////////////////////////////

StreakEffect::StreakEffect(RenderContext const& ctx)
: streak_shader_(R"(
    // vertex shader ---------------------------------------------------------
    @include "version"

    layout(location=0) in vec2 position;

    uniform vec2 step;

    out vec2 texcoords[4];

    void main(void){
      texcoords[0] = vec2(position.x + 1.0, 1.0 + position.y) * 0.5;
      texcoords[1] = texcoords[0] + step;
      texcoords[2] = texcoords[0] + step * 2.0;
      texcoords[3] = texcoords[0] + step * 3.0;
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )", R"(
    // fragment shader -------------------------------------------------------
    @include "version"

    uniform sampler2D input_tex;
    uniform vec3 colors[4];

    in vec2 texcoords[4];

    layout (location = 0) out vec3 fragColor;

    void main(void) {
      fragColor = vec3(0);
      for (int i=0; i<4; ++i) {
        fragColor += texture(input_tex, texcoords[i]).rgb * colors[i];
      }
    }
  )")
  , step_(streak_shader_.get_uniform<math::vec2>("step"))
  , input_tex_(streak_shader_.get_uniform<int>("input_tex"))
  , colors_(streak_shader_.get_uniform<math::vec3>("colors"))  {

  auto create_texture = [&](
    ogl::Texture& tex, int width, int height,
    ogl::enums::PixelDataInternalFormat i_format,
    ogl::enums::PixelDataFormat         p_format) {

    ogl::Context::Bound(ogl::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, ogl::PixelDataType::UnsignedByte, nullptr)
      .MaxLevel(0)
      .MinFilter(ogl::TextureMinFilter::Linear)
      .MagFilter(ogl::TextureMagFilter::Linear)
      .WrapS(ogl::TextureWrap::ClampToBorder)
      .WrapT(ogl::TextureWrap::ClampToBorder);
  };

  auto size(ctx.g_buffer_size/GBUFFER_FRACTION);

  create_texture(
    streak_buffer_tmp_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_1_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_2_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_3_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_4_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_5_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    streak_buffer_6_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
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

  ogl::Context::Viewport(ctx.g_buffer_size.x()/GBUFFER_FRACTION, ctx.g_buffer_size.y()/GBUFFER_FRACTION);

  streak_fbo_.Bind(ogl::Framebuffer::Target::Draw);
  const float DEC = 0.96;
  streak_shader_.use();
  input_tex_.Set(4);
  ogl::Texture::Active(4);

  auto pass = [&](math::vec2 const& step_dir, ogl::Texture const& input,
                  ogl::FramebufferColorAttachment output,
                  std::vector<math::vec3>& colors_in) {

    ogl::Context::DrawBuffer(output);
    ogl::Context::Bind(ose::_2D(), input);

    colors_.Set(colors_in);
    step_.Set(step_dir);

    Quad::get().draw();
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

  // step_dir = math::vec2(-0.7*4.0/ctx.g_buffer_size.x(), -0.9*4.0/ctx.g_buffer_size.y());
  // pass(step_dir*7, threshold_buffer_,  ogl::FramebufferColorAttachment::_0, streak_colors_4_);
  // pass(step_dir*3,  streak_buffer_tmp_, ogl::FramebufferColorAttachment::_3, streak_colors_3_);

  // step_dir = math::vec2(0.7*4.0/ctx.g_buffer_size.x(), -0.9*4.0/ctx.g_buffer_size.y());
  // pass(step_dir*7, threshold_buffer_,  ogl::FramebufferColorAttachment::_0, streak_colors_4_);
  // pass(step_dir*3,  streak_buffer_tmp_, ogl::FramebufferColorAttachment::_4, streak_colors_3_);

  // step_dir = math::vec2(-0.7*4.0/ctx.g_buffer_size.x(), 0.9*4.0/ctx.g_buffer_size.y());
  // pass(step_dir*7, threshold_buffer_,  ogl::FramebufferColorAttachment::_0, streak_colors_4_);
  // pass(step_dir*3,  streak_buffer_tmp_, ogl::FramebufferColorAttachment::_5, streak_colors_3_);

  // step_dir = math::vec2(0.7*4.0/ctx.g_buffer_size.x(), 0.9*4.0/ctx.g_buffer_size.y());
  // pass(step_dir*7, threshold_buffer_,  ogl::FramebufferColorAttachment::_0, streak_colors_4_);
  // pass(step_dir*3,  streak_buffer_tmp_, ogl::FramebufferColorAttachment::_6, streak_colors_3_);
}

////////////////////////////////////////////////////////////////////////////////

int StreakEffect::bind_buffers(int start, RenderContext const& ctx) {
  ogl::Texture::Active(start + 0);
  ogl::Context::Bind(ose::_2D(), streak_buffer_1_);

  ogl::Texture::Active(start + 1);
  ogl::Context::Bind(ose::_2D(), streak_buffer_2_);

  // ogl::Texture::Active(start + 2);
  // ogl::Context::Bind(ose::_2D(), streak_buffer_3_);

  // ogl::Texture::Active(start + 3);
  // ogl::Context::Bind(ose::_2D(), streak_buffer_4_);

  // ogl::Texture::Active(start + 4);
  // ogl::Context::Bind(ose::_2D(), streak_buffer_5_);

  // ogl::Texture::Active(start + 5);
  // ogl::Context::Bind(ose::_2D(), streak_buffer_6_);

  return start + 2;
  // return start + 6;
}

////////////////////////////////////////////////////////////////////////////////

}
