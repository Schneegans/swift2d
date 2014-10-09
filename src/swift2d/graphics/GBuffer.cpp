////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/GBuffer.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

GBuffer::GBuffer(RenderContext const& ctx, bool final_buffer, bool diffuse_alpha,
                 int sub_sample_level)
  : sub_sample_level_(sub_sample_level) {

  auto create_texture = [&](
    oglplus::Texture& tex, int width, int height,
    oglplus::enums::PixelDataInternalFormat i_format,
    oglplus::enums::PixelDataFormat         p_format) {

    ctx.gl.Bound(oglplus::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, oglplus::PixelDataType::UnsignedByte, nullptr)
      .MinFilter((ctx.sub_sampling || sub_sample_level_ > 1) ? oglplus::TextureMinFilter::Linear : oglplus::TextureMinFilter::Nearest)
      .MagFilter((ctx.sub_sampling || sub_sample_level_ > 1) ? oglplus::TextureMagFilter::Linear : oglplus::TextureMagFilter::Nearest)
      .WrapS(oglplus::TextureWrap::MirroredRepeat)
      .WrapT(oglplus::TextureWrap::MirroredRepeat);
  };

  // create textures for G-Buffer and L-Buffer -------------------------------
  auto size(ctx.g_buffer_size/sub_sample_level);

  if (diffuse_alpha) {
    create_texture(
      diffuse_, size.x(), size.y(),
      oglplus::PixelDataInternalFormat::RGBA8,
      oglplus::PixelDataFormat::RGBA
    );
  } else {
    create_texture(
      diffuse_, size.x(), size.y(),
      oglplus::PixelDataInternalFormat::RGB8,
      oglplus::PixelDataFormat::RGB
    );
  }

  if (ctx.shading_quality > 0) {
    create_texture(
      normal_, size.x(), size.y(),
      oglplus::PixelDataInternalFormat::RGB8,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      light_, size.x(), size.y(),
      oglplus::PixelDataInternalFormat::RGB8,
      oglplus::PixelDataFormat::RGB
    );

    if (final_buffer) {
      create_texture(
        final_, size.x(), size.y(),
        oglplus::PixelDataInternalFormat::RGB8,
        oglplus::PixelDataFormat::RGB
      );
    }
  }

  // create framebuffer object -----------------------------------------------
  fbo_.Bind(oglplus::Framebuffer::Target::Draw);

  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 0, diffuse_, 0
  );

  if (ctx.shading_quality > 0) {
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 1, normal_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 2, light_, 0
    );
    if (final_buffer) {
      oglplus::Framebuffer::AttachColorTexture(
        oglplus::Framebuffer::Target::Draw, 3, final_, 0
      );
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void GBuffer::bind_for_drawing(RenderContext const& ctx, bool clear) {

  auto size(ctx.g_buffer_size/sub_sample_level_);
  ctx.gl.Viewport(size.x(), size.y());

  if (ctx.shading_quality == 0) {

    fbo_.Bind(oglplus::Framebuffer::Target::Draw);

    oglplus::Context::ColorBuffer draw_buffs[1] =  {
      oglplus::FramebufferColorAttachment::_0
    };
    ctx.gl.DrawBuffers(draw_buffs);

    if (clear) {
      ctx.gl.ClearColor(0.f, 0.f, 0.f, 0.f);
      ctx.gl.Clear().ColorBuffer();
    }

  } else {

    fbo_.Bind(oglplus::Framebuffer::Target::Draw);

    oglplus::Context::ColorBuffer draw_buffs[3] =  {
      oglplus::FramebufferColorAttachment::_0,
      oglplus::FramebufferColorAttachment::_1,
      oglplus::FramebufferColorAttachment::_2
    };
    ctx.gl.DrawBuffers(draw_buffs);

    if (clear) {
      ctx.gl.ClearColor(0.f, 0.f, 0.f, 0.f);
      ctx.gl.Clear().ColorBuffer();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void GBuffer::bind_final_buffer_for_drawing(RenderContext const& ctx, bool clear) {
  ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_3);

  if (clear) {
    GLfloat clear[3] = {0.f, 0.f, 0.f};
    ctx.gl.ClearColorBuffer(0, clear);
  }
}

////////////////////////////////////////////////////////////////////////////////

void GBuffer::bind_diffuse(int location) {
  oglplus::Texture::Active(location);
  diffuse_.Bind(oglplus::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

void GBuffer::bind_normal(int location) {
  oglplus::Texture::Active(location);
  normal_.Bind(oglplus::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

void GBuffer::bind_light(int location) {
  oglplus::Texture::Active(location);
  light_.Bind(oglplus::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

void GBuffer::bind_final(int location) {
  oglplus::Texture::Active(location);
  final_.Bind(oglplus::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

}
