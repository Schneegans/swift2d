////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/HeatEffect.hpp>

#include <swift2d/components/DrawableComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HeatEffect::HeatEffect(RenderContext const& ctx, int shading_quality) {
  if (shading_quality > 2) {

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

void HeatEffect::process(ConstSerializedScenePtr const& scene,
                         RenderContext const& ctx) {

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

////////////////////////////////////////////////////////////////////////////////

int HeatEffect::bind_buffers(int start, RenderContext const& ctx) {
  oglplus::Texture::Active(start);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), heat_buffer_);
  return start + 1;
}

////////////////////////////////////////////////////////////////////////////////

}
