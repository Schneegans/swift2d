////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/GhostEffect.hpp>

#include <swift2d/geometries/Quad.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

GhostEffect::GhostEffect(RenderContext const& ctx)
: blur_shader_(R"(
    // vertex shader ---------------------------------------------------------
    @include "version"

    layout(location=0) in vec2 position;

    uniform vec2 step;

    out vec2 texcoord1;
    out vec2 texcoord2;
    out vec2 texcoord3;
    out vec2 texcoord4;
    out vec2 texcoord5;

    void main(void){
      texcoord3 = vec2(position.x + 1.0, 1.0 + position.y) * 0.5;
      texcoord1 = texcoord3 + step * -2.0;
      texcoord2 = texcoord3 + step * -1.0;
      texcoord4 = texcoord3 + step *  1.0;
      texcoord5 = texcoord3 + step *  2.0;
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )", R"(
    // fragment shader -------------------------------------------------------
    @include "version"

    uniform sampler2D input_tex;

    in vec2 texcoord1;
    in vec2 texcoord2;
    in vec2 texcoord3;
    in vec2 texcoord4;
    in vec2 texcoord5;

    layout (location = 0) out vec3 fragColor;

    void main(void) {
      fragColor = texture(input_tex, texcoord1).rgb * 0.1
                + texture(input_tex, texcoord2).rgb * 0.2
                + texture(input_tex, texcoord3).rgb * 0.4
                + texture(input_tex, texcoord4).rgb * 0.2
                + texture(input_tex, texcoord5).rgb * 0.1;
    }
  )")
  , ghost_shader_(R"(
    // vertex shader ---------------------------------------------------------
    @include "version"

    layout(location=0) in vec2 position;

    uniform vec4 scalar;

    out vec2 texcoords[4];

    void main(void){
      vec2 t = vec2(position.x + 1.0, 1.0 + position.y) * 0.5;
      for (int i=0; i<4; ++i) {
        texcoords[i] = (t - 0.5) * scalar[i] + 0.5;
      }
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )", R"(
    // fragment shader -------------------------------------------------------
    @include "version"

    in vec2 texcoords[4];

    uniform sampler2D inputs[3];
    uniform vec3 colors[4];

    layout (location = 0) out vec3 fragColor;

    void main(void) {
      fragColor = vec3(0);
      for (int i=0; i<4; ++i) {
        vec2 fac = -pow((texcoords[i]-0.5)*2, vec2(2)) + 1;
        fragColor += texture(inputs[max(0, i-1)], texcoords[i]).rgb * colors[i] * fac.x * fac.y;
      }
    }
  )")
  , step_(blur_shader_.get_uniform<math::vec2>("step"))
  , input_tex_(blur_shader_.get_uniform<int>("input_tex"))
  , scalar_(ghost_shader_.get_uniform<math::vec4>("scalar"))
  , inputs_(ghost_shader_.get_uniform<int>("inputs"))
  , colors_(ghost_shader_.get_uniform<math::vec3>("colors")) {

  auto create_texture = [&](
    ogl::Texture& tex, int width, int height,
    ogl::enums::PixelDataInternalFormat i_format,
    ogl::enums::PixelDataFormat         p_format) {

    ctx.gl.Bound(ogl::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, ogl::PixelDataType::UnsignedByte, nullptr)
      .MaxLevel(0)
      .MinFilter(ogl::TextureMinFilter::Linear)
      .MagFilter(ogl::TextureMagFilter::Linear)
      .WrapS(ogl::TextureWrap::ClampToBorder)
      .WrapT(ogl::TextureWrap::ClampToBorder);
  };

  auto size(ctx.g_buffer_size/6);

  create_texture(
    buffer_tmp_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    blur_buffer_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    ghost_buffer_1_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    ghost_buffer_2_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );


  fbo_.Bind(ogl::Framebuffer::Target::Draw);
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 0, buffer_tmp_, 0
  );
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 1, blur_buffer_, 0
  );
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 2, ghost_buffer_1_, 0
  );
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 3, ghost_buffer_2_, 0
  );

}

////////////////////////////////////////////////////////////////////////////////

void GhostEffect::process(RenderContext const& ctx, ogl::Texture const& threshold_buffer_) {

  ctx.gl.Viewport(ctx.g_buffer_size.x()/6, ctx.g_buffer_size.y()/6);

  fbo_.Bind(ogl::Framebuffer::Target::Draw);

  // blurring ------------------------------------------------------------------
  blur_shader_.use(ctx);
  input_tex_.Set(4);
  ogl::Texture::Active(4);

  auto blur_pass = [&](math::vec2 const& step, ogl::Texture const& input,
                  ogl::FramebufferColorAttachment output) {

    ctx.gl.DrawBuffer(output);
    ctx.gl.Bind(ose::_2D(), input);
    step_.Set(step);
    Quad::get().draw(ctx);
  };

  math::vec2 radius(2.0/(ctx.g_buffer_size.x()/6), 2.0/(ctx.g_buffer_size.y()/6));

  blur_pass(math::vec2(radius.x()/2, 0), threshold_buffer_, ogl::FramebufferColorAttachment::_0);
  blur_pass(math::vec2(radius.x(), 0),   buffer_tmp_,       ogl::FramebufferColorAttachment::_1);
  blur_pass(math::vec2(0, radius.y()/2), blur_buffer_,      ogl::FramebufferColorAttachment::_0);
  blur_pass(math::vec2(0, radius.y()),   buffer_tmp_,       ogl::FramebufferColorAttachment::_1);

  // ghosting ------------------------------------------------------------------
  ctx.gl.DrawBuffer(ogl::FramebufferColorAttachment::_2);

  ghost_shader_.use(ctx);

  ogl::Texture::Active(4);
  ctx.gl.Bind(ose::_2D(), blur_buffer_);

  ogl::Texture::Active(5);
  ctx.gl.Bind(ose::_2D(), blur_buffer_);

  ogl::Texture::Active(6);
  ctx.gl.Bind(ose::_2D(), blur_buffer_);

  std::vector<int> inputs = {4, 5, 6};
  inputs_.Set(inputs);

  std::vector<math::vec3> colors = {
    math::vec3(0.8, 0.5, 0.5), math::vec3(1.0, 0.2, 0.6),
    math::vec3(0.1, 0.1, 0.4), math::vec3(0.0, 0.0, 0.5)
  };
  colors_.Set(math::vec3(0.8, 0.5, 0.5));
  scalar_.Set(math::vec4(-4.0, 3.0, -2.0, 0.3));

  Quad::get().draw(ctx);


  ctx.gl.DrawBuffer(ogl::FramebufferColorAttachment::_3);

  ogl::Texture::Active(4);
  ctx.gl.Bind(ose::_2D(), ghost_buffer_1_);

  ogl::Texture::Active(5);
  ctx.gl.Bind(ose::_2D(), ghost_buffer_1_);

  ogl::Texture::Active(6);
  ctx.gl.Bind(ose::_2D(), blur_buffer_);

  colors = {
    math::vec3(0.6, 0.2, 0.2), math::vec3(0.2, 0.06, 0.6),
    math::vec3(0.15, 0.00, 0.1), math::vec3(0.06, 0.00, 0.55)
  };
  colors_.Set(colors);
  scalar_.Set(math::vec4(3.6, 2.0, 0.9, -0.55));

  Quad::get().draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

int GhostEffect::bind_buffers(int start, RenderContext const& ctx) {
  ogl::Texture::Active(start + 0);
  ctx.gl.Bind(ose::_2D(), ghost_buffer_1_);

  ogl::Texture::Active(start + 1);
  ctx.gl.Bind(ose::_2D(), ghost_buffer_2_);

  return start + 2;
}

////////////////////////////////////////////////////////////////////////////////

}
