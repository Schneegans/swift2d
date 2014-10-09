////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/LBuffer.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LBuffer::LBuffer(RenderContext const& ctx) {

  auto create_texture = [&](
    ogl::Texture& tex, int width, int height,
    ogl::enums::PixelDataInternalFormat i_format,
    ogl::enums::PixelDataFormat         p_format) {

    ctx.gl.Bound(ogl::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, ogl::PixelDataType::UnsignedByte, nullptr)
      .MinFilter(ogl::TextureMinFilter::Linear)
      .MagFilter(ogl::TextureMagFilter::Linear)
      .WrapS(ogl::TextureWrap::MirroredRepeat)
      .WrapT(ogl::TextureWrap::MirroredRepeat);
  };

  // create textures -----------------------------------------------------------
  if (ctx.shading_quality > 0) {
    auto size(ctx.g_buffer_size/(ctx.shading_quality > 4 ? 1 : 4));

    create_texture(
      buffer_, size.x(), size.y(),
      ogl::PixelDataInternalFormat::RGBA8,
      ogl::PixelDataFormat::RGBA
    );

    // create framebuffer object -------------------------------------------------
    fbo_.Bind(ogl::Framebuffer::Target::Draw);

    ogl::Framebuffer::AttachColorTexture(
      ogl::Framebuffer::Target::Draw, 0, buffer_, 0
    );
  }
}

////////////////////////////////////////////////////////////////////////////////

void LBuffer::bind_for_drawing(RenderContext const& ctx) {

  if (ctx.shading_quality > 0) {
    auto size(ctx.g_buffer_size/(ctx.shading_quality > 4 ? 1 : 4));
    ctx.gl.Viewport(size.x(), size.y());

    fbo_.Bind(ogl::Framebuffer::Target::Draw);

    ogl::Context::ColorBuffer draw_buffs[1] =  {
      ogl::FramebufferColorAttachment::_0
    };
    ctx.gl.DrawBuffers(draw_buffs);
  }
}

////////////////////////////////////////////////////////////////////////////////

void LBuffer::bind(int location) {
  ogl::Texture::Active(location);
  buffer_.Bind(ogl::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

}
