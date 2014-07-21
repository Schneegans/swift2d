////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Compositor.hpp>
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/gui/Interface.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Compositor::Compositor(RenderContext const& ctx, int shading_quality)
  : shading_quality_ (shading_quality)
  , shader_(nullptr)
  , fbo_()
  , g_buffer_diffuse_ ()
  , g_buffer_normal_ ()
  , g_buffer_light_()
  , final_buffer_  ()
  , post_processor_(nullptr) {

  auto create_texture = [&](
    oglplus::Texture& tex, int width, int height,
    oglplus::enums::PixelDataInternalFormat i_format,
    oglplus::enums::PixelDataFormat         p_format) {

    oglplus::Texture::Active(0);
    ctx.gl.Bound(oglplus::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, oglplus::PixelDataType::Float, nullptr)
      .MinFilter(oglplus::TextureMinFilter::Nearest)
      .MagFilter(oglplus::TextureMagFilter::Nearest)
      .WrapS(oglplus::TextureWrap::MirroredRepeat)
      .WrapT(oglplus::TextureWrap::MirroredRepeat);
  };

  if (shading_quality_ > 0) {

    // create textures for G-Buffer and L-Buffer -------------------------------
    create_texture(
      g_buffer_diffuse_, ctx.size.x(), ctx.size.y(),
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      g_buffer_normal_, ctx.size.x(), ctx.size.y(),
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      g_buffer_light_, ctx.size.x(), ctx.size.y(),
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      final_buffer_, ctx.size.x(), ctx.size.y(),
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
    );

    // create framebuffer object -----------------------------------------------
    fbo_.Bind(oglplus::Framebuffer::Target::Draw);

    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 0, g_buffer_diffuse_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 1, g_buffer_normal_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 2, g_buffer_light_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 3, final_buffer_, 0
    );

    shader_ = new Shader(
      R"(
        // vertex shader -------------------------------------------------------
        @include "fullscreen_quad_vertext_shader"
      )",
      R"(
        // fragment shader -----------------------------------------------------
        @include "version"

        @include "gbuffer_input"

        layout (location = 0) out vec3 fragColor;

        void main(void){
          fragColor = get_light_info().r * get_diffuse();
        }
    )");

    if (shading_quality_ > 1) {
      post_processor_ = new PostProcessor(ctx, shading_quality_);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

Compositor::~Compositor() {
  if(post_processor_) delete post_processor_;
  if(shader_)         delete shader_;
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_objects(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {

  ctx.gl.BlendFunc(
    oglplus::BlendFunction::SrcAlpha,
    oglplus::BlendFunction::OneMinusSrcAlpha
  );

  ctx.gl.Viewport(ctx.size.x(), ctx.size.y());

  if (shading_quality_ > 0) {

    fbo_.Bind(oglplus::Framebuffer::Target::Draw);

    oglplus::Context::ColorBuffer draw_buffs[3] =  {
      oglplus::FramebufferColorAttachment::_0,
      oglplus::FramebufferColorAttachment::_1,
      oglplus::FramebufferColorAttachment::_2
    };
    ctx.gl.DrawBuffers(draw_buffs);

    GLfloat clear0[3] = {0.f, 0.f, 0.f};
    ctx.gl.ClearColorBuffer(0, clear0);

    GLfloat clear1[3] = {0.5f, 0.5f, 0.f};
    ctx.gl.ClearColorBuffer(1, clear1);

    GLfloat clear2[3] = {1.f, 1.f, 0.f};
    ctx.gl.ClearColorBuffer(2, clear2);

  } else {
    oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);
    ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);
    ctx.gl.Clear().ColorBuffer();
  }

  for (auto& object: scene->objects) {
    object.second->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_lights(ConstSerializedScenePtr const& scene,
                             RenderContext const& ctx) {

  if (shading_quality_ > 0) {
    ctx.gl.BlendFunc(
      oglplus::BlendFunction::One,
      oglplus::BlendFunction::OneMinusSrcColor
    );

    if (shading_quality_ > 1) {
      ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_3);
    } else {
      oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);
      ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);
    }

    GLfloat clear[3] = {0.f, 0.f, 0.f};
    ctx.gl.ClearColorBuffer(0, clear);

    oglplus::Texture::Active(1);
    g_buffer_diffuse_.Bind(oglplus::Texture::Target::_2D);

    oglplus::Texture::Active(2);
    g_buffer_normal_.Bind(oglplus::Texture::Target::_2D);

    oglplus::Texture::Active(3);
    g_buffer_light_.Bind(oglplus::Texture::Target::_2D);

    shader_->use(ctx);
    shader_->set_uniform("screen_size", ctx.size);
    shader_->set_uniform("g_buffer_diffuse", 1);
    shader_->set_uniform("g_buffer_light", 3);
    Quad::instance()->draw(ctx);

    for (auto& light: scene->lights) {
      light.second->draw(ctx);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::post_process(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {

  if (shading_quality_ > 1) {
    post_processor_->process(scene, ctx, final_buffer_, g_buffer_light_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_gui(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  for (auto& gui: scene->gui_elements) {
    gui.second->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
