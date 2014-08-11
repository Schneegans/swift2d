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
      fragColor = texture2D(input_tex, texcoord1).rgb * 0.1
                + texture2D(input_tex, texcoord2).rgb * 0.2
                + texture2D(input_tex, texcoord3).rgb * 0.4
                + texture2D(input_tex, texcoord4).rgb * 0.2
                + texture2D(input_tex, texcoord5).rgb * 0.1;
    }
  )")
  , ghost_shader_(R"(
    // vertex shader ---------------------------------------------------------
    @include "version"

    layout(location=0) in vec2 position;

    uniform vec4 scalar;

    out vec2 texcoord1;
    out vec2 texcoord2;
    out vec2 texcoord3;
    out vec2 texcoord4;

    void main(void){
      vec2 texcoord = vec2(position.x + 1.0, 1.0 + position.y) * 0.5;
      texcoord1 = (texcoord - 0.5) * scalar[0] + 0.5;
      texcoord2 = (texcoord - 0.5) * scalar[1] + 0.5;
      texcoord3 = (texcoord - 0.5) * scalar[2] + 0.5;
      texcoord4 = (texcoord - 0.5) * scalar[3] + 0.5;
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )", R"(
    // fragment shader -------------------------------------------------------
    @include "version"

    in vec2 texcoord1;
    in vec2 texcoord2;
    in vec2 texcoord3;
    in vec2 texcoord4;

    uniform sampler2D input_tex1;
    uniform sampler2D input_tex2;
    uniform sampler2D input_tex3;

    uniform vec3 color1;
    uniform vec3 color2;
    uniform vec3 color3;
    uniform vec3 color4;

    layout (location = 0) out vec3 fragColor;

    void main(void) {
      vec2 fac1 = -pow((texcoord1-0.5)*2, vec2(2)) + 1;
      vec2 fac2 = -pow((texcoord2-0.5)*2, vec2(2)) + 1;
      vec2 fac3 = -pow((texcoord3-0.5)*2, vec2(2)) + 1;
      vec2 fac4 = -pow((texcoord4-0.5)*2, vec2(2)) + 1;

      fragColor = texture2D(input_tex1, texcoord1).rgb * color1 * fac1.x * fac1.y
                + texture2D(input_tex1, texcoord2).rgb * color2 * fac2.x * fac2.y
                + texture2D(input_tex2, texcoord3).rgb * color3 * fac3.x * fac3.y
                + texture2D(input_tex3, texcoord4).rgb * color4 * fac4.x * fac4.y;
    }
  )") {

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
    buffer_tmp_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    blur_buffer_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    ghost_buffer_1_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
    ogl::PixelDataFormat::RGB
  );

  create_texture(
    ghost_buffer_2_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB,
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
  blur_shader_.set_uniform("input_tex", 4);
  ogl::Texture::Active(4);

  auto blur_pass = [&](math::vec2 const& step, ogl::Texture const& input,
                  ogl::FramebufferColorAttachment output) {

    ctx.gl.DrawBuffer(output);
    ctx.gl.Bind(ose::_2D(), input);
    blur_shader_.set_uniform("step", step);
    Quad::instance()->draw(ctx);
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
  ghost_shader_.set_uniform("input_tex1", 4);

  ogl::Texture::Active(5);
  ctx.gl.Bind(ose::_2D(), blur_buffer_);
  ghost_shader_.set_uniform("input_tex2", 5);

  ogl::Texture::Active(6);
  ctx.gl.Bind(ose::_2D(), blur_buffer_);
  ghost_shader_.set_uniform("input_tex3", 6);

  ghost_shader_.set_uniform("scalar", math::vec4(-4.0, 3.0, -2.0, 0.3));
  ghost_shader_.set_uniform("color1", math::vec3(0.8, 0.5, 0.5));
  ghost_shader_.set_uniform("color2", math::vec3(1.0, 0.2, 0.6));
  ghost_shader_.set_uniform("color3", math::vec3(0.1, 0.1, 0.4));
  ghost_shader_.set_uniform("color4", math::vec3(0.0, 0.0, 0.5));

  Quad::instance()->draw(ctx);



  ctx.gl.DrawBuffer(ogl::FramebufferColorAttachment::_3);

  ogl::Texture::Active(4);
  ctx.gl.Bind(ose::_2D(), ghost_buffer_1_);
  ghost_shader_.set_uniform("input_tex1", 4);

  ogl::Texture::Active(5);
  ctx.gl.Bind(ose::_2D(), ghost_buffer_1_);
  ghost_shader_.set_uniform("input_tex2", 5);

  ogl::Texture::Active(6);
  ctx.gl.Bind(ose::_2D(), blur_buffer_);
  ghost_shader_.set_uniform("input_tex3", 6);

  ghost_shader_.set_uniform("scalar", math::vec4(3.6, 2.0, 0.9, -0.55));
  ghost_shader_.set_uniform("color1", math::vec3(0.6, 0.2, 0.2));
  ghost_shader_.set_uniform("color2", math::vec3(0.2, 0.06, 0.6));
  ghost_shader_.set_uniform("color3", math::vec3(0.15, 0.00, 0.1));
  ghost_shader_.set_uniform("color4", math::vec3(0.06, 0.00, 0.55));

  Quad::instance()->draw(ctx);
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
