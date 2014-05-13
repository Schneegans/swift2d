////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Pipeline.hpp>

#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/components/CameraComponent.hpp>
#include <swift2d/graphics/Window.hpp>
#include <swift2d/utils/Logger.hpp>
#include <thread>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>


namespace swift {

////////////////////////////////////////////////////////////////////////////////

Pipeline::Pipeline()
  : old_size_(-1, -1)
  , fbo_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::set_output_window(WindowPtr const& window) {
  window_ = window;

  fullscreen_quad_ = std::make_shared<SpriteResource>();
}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::draw(ConstSerializedScenePtr const& scene) {

  auto update_fbo = [&](){

    if (fbo_)               delete fbo_;
    if (offscreen_color_)   delete offscreen_color_;
    if (offscreen_normal_)  delete offscreen_normal_;

    // Texture::Active(BumpMapUnit());
    offscreen_color_ = new oglplus::Texture();

    oglplus::Texture::Active(2);
    window_->get_context().gl.Bound(oglplus::Texture::Target::_2D, *offscreen_color_)
      .MinFilter(oglplus::TextureMinFilter::Nearest)
      .MagFilter(oglplus::TextureMagFilter::Nearest)
      .WrapS(oglplus::TextureWrap::ClampToEdge)
      .WrapT(oglplus::TextureWrap::ClampToEdge)
      .Image2D(
        0,
        oglplus::PixelDataInternalFormat::RGBA,
        window_->get_context().size.x(), window_->get_context().size.y(),
        0,
        oglplus::PixelDataFormat::RGBA,
        oglplus::PixelDataType::Float,
        nullptr
      );

    offscreen_normal_ = new oglplus::Texture();
    oglplus::Texture::Active(3);
    window_->get_context().gl.Bound(oglplus::Texture::Target::_2D, *offscreen_normal_)
      .MinFilter(oglplus::TextureMinFilter::Nearest)
      .MagFilter(oglplus::TextureMagFilter::Nearest)
      .WrapS(oglplus::TextureWrap::ClampToEdge)
      .WrapT(oglplus::TextureWrap::ClampToEdge)
      .Image2D(
        0,
        oglplus::PixelDataInternalFormat::RGBA,
        window_->get_context().size.x(), window_->get_context().size.y(),
        0,
        oglplus::PixelDataFormat::RGBA,
        oglplus::PixelDataType::Float,
        nullptr
      );

    offscreen_light_ = new oglplus::Texture();
    oglplus::Texture::Active(4);
    window_->get_context().gl.Bound(oglplus::Texture::Target::_2D, *offscreen_light_)
      .MinFilter(oglplus::TextureMinFilter::Nearest)
      .MagFilter(oglplus::TextureMagFilter::Nearest)
      .WrapS(oglplus::TextureWrap::ClampToEdge)
      .WrapT(oglplus::TextureWrap::ClampToEdge)
      .Image2D(
        0,
        oglplus::PixelDataInternalFormat::RGBA,
        window_->get_context().size.x(), window_->get_context().size.y(),
        0,
        oglplus::PixelDataFormat::RGBA,
        oglplus::PixelDataType::Float,
        nullptr
      );

    fbo_ = new oglplus::Framebuffer();
    fbo_->Bind(oglplus::Framebuffer::Target::Draw);
    oglplus::Framebuffer::AttachColorTexture(oglplus::Framebuffer::Target::Draw, 0, *offscreen_color_, 0);
    oglplus::Framebuffer::AttachColorTexture(oglplus::Framebuffer::Target::Draw, 1, *offscreen_normal_, 0);
    oglplus::Framebuffer::AttachColorTexture(oglplus::Framebuffer::Target::Draw, 2, *offscreen_light_, 0);
  };


  // create & update window ----------------------------------------------------
  if (!window_->Open()) {
    window_->Open = true;
  }

  if (old_size_ != window_->get_context().size) {
    old_size_ = window_->get_context().size;
    window_->get_context().gl.Viewport(old_size_.x(), old_size_.y());

    update_fbo();
  }

  // setup projection matrix ---------------------------------------------------
  math::mat3 view_matrix(scene->camera->WorldTransform.get());
  math::scale(view_matrix, scene->camera->Size.get());
  window_->get_context().projection_matrix = math::inversed(view_matrix);

  // draw opaque objects -------------------------------------------------------

  window_->get_context().gl.BlendFunc(
    oglplus::BlendFunction::SrcAlpha,
    oglplus::BlendFunction::OneMinusSrcAlpha
  );

  fbo_->Bind(oglplus::Framebuffer::Target::Draw);

  oglplus::Context::ColorBuffer draw_buffs[2] =  {
    oglplus::FramebufferColorAttachment::_0,
    oglplus::FramebufferColorAttachment::_1
  };
  window_->get_context().gl.DrawBuffers(draw_buffs);

  for (auto& object: scene->objects) {
    object.second->draw(window_->get_context());
  }

  // draw lights ---------------------------------------------------------------

  window_->get_context().gl.BlendFunc(
    oglplus::BlendFunction::One,
    oglplus::BlendFunction::One
  );

  window_->get_context().gl.DrawBuffer(oglplus::FramebufferColorAttachment::_2);

  GLfloat clear[4] = {0.f, 0.f, 0.f, 0.f};
  window_->get_context().gl.ClearColorBuffer(0, clear);

  for (auto& light: scene->lights) {
    light.second->draw(window_->get_context());
  }

  // draw transparent objects --------------------------------------------------
  oglplus::DefaultFramebuffer::Bind(oglplus::Framebuffer::Target::Draw);
  window_->get_context().gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);

  oglplus::Texture::Active(0);
  window_->get_context().gl.Bind(oglplus::smart_enums::_2D(), *offscreen_light_);

  fullscreen_quad_->draw(window_->get_context(), math::make_scale(2), false);

  // finish frame --------------------------------------------------------------

  window_->display();
}

////////////////////////////////////////////////////////////////////////////////

}
