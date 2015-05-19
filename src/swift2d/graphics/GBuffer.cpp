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
#include <swift2d/graphics/GBuffer.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

GBuffer::GBuffer(RenderContext const& ctx) {

  auto create_texture = [&](
    ogl::Texture& tex, int width, int height,
    ogl::enums::PixelDataInternalFormat i_format,
    ogl::enums::PixelDataFormat         p_format) {

    ogl::Context::Bound(ogl::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, ogl::PixelDataType::UnsignedByte, nullptr)
      .MaxLevel(0)
      .MinFilter(ctx.sub_sampling ? ogl::TextureMinFilter::Linear : ogl::TextureMinFilter::Nearest)
      .MagFilter(ctx.sub_sampling ? ogl::TextureMagFilter::Linear : ogl::TextureMagFilter::Nearest)
      .WrapS(ogl::TextureWrap::MirroredRepeat)
      .WrapT(ogl::TextureWrap::MirroredRepeat);
  };

  auto size(ctx.g_buffer_size);

  // create textures -----------------------------------------------------------
  create_texture(
    diffuse_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  if (ctx.dynamic_lighting) {
    create_texture(
      normal_, size.x(), size.y(),
      ogl::PixelDataInternalFormat::RGB8,
      ogl::PixelDataFormat::RGB
    );
    create_texture(
      light_, size.x(), size.y(),
      ogl::PixelDataInternalFormat::RGB8,
      ogl::PixelDataFormat::RGB
    );
  }

  // create framebuffer object -------------------------------------------------
  fbo_.Bind(ogl::Framebuffer::Target::Draw);

  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 0, diffuse_, 0
  );

  if (ctx.dynamic_lighting) {
    ogl::Framebuffer::AttachColorTexture(
      ogl::Framebuffer::Target::Draw, 1, normal_, 0
    );
    ogl::Framebuffer::AttachColorTexture(
      ogl::Framebuffer::Target::Draw, 2, light_, 0
    );
  }
}

////////////////////////////////////////////////////////////////////////////////

void GBuffer::bind_for_drawing(RenderContext const& ctx) {

  auto size(ctx.g_buffer_size);
  ogl::Context::Viewport(size.x(), size.y());

  if (ctx.dynamic_lighting) {
    fbo_.Bind(ogl::Framebuffer::Target::Draw);

    ogl::Context::ColorBuffer draw_buffs[3] =  {
      ogl::FramebufferColorAttachment::_0,
      ogl::FramebufferColorAttachment::_1,
      ogl::FramebufferColorAttachment::_2
    };
    ogl::Context::DrawBuffers(draw_buffs);
  } else {
    fbo_.Bind(ogl::Framebuffer::Target::Draw);

    ogl::Context::ColorBuffer draw_buffs[1] =  {
      ogl::FramebufferColorAttachment::_0
    };
    ogl::Context::DrawBuffers(draw_buffs);
  }
}

////////////////////////////////////////////////////////////////////////////////

void GBuffer::bind_diffuse(int location) {
  ogl::Texture::Active(location);
  diffuse_.Bind(ogl::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

void GBuffer::bind_normal(int location) {
  ogl::Texture::Active(location);
  normal_.Bind(ogl::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

void GBuffer::bind_light(int location) {
  ogl::Texture::Active(location);
  light_.Bind(ogl::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

}
