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

Compositor::Compositor()
  : ShadingQuality    (0)
  , shader_           (nullptr)
  , fbo_              (nullptr)
  , offscreen_color_  (nullptr)
  , offscreen_normal_ (nullptr)
  , offscreen_light_  (nullptr)
  , offscreen_aux_1_  (nullptr)
  , offscreen_aux_2_  (nullptr)
  , post_processor_   (nullptr) {}

////////////////////////////////////////////////////////////////////////////////

Compositor::~Compositor() {
  clean_up();
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::init(RenderContext const& ctx) {

  clean_up();

  if (ShadingQuality() > 0) {

    // create textures for G-Buffer and L-Buffer -------------------------------
    offscreen_color_  = new oglplus::Texture();
    offscreen_normal_ = new oglplus::Texture();
    offscreen_light_  = new oglplus::Texture();
    offscreen_aux_1_  = new oglplus::Texture();
    offscreen_aux_2_  = new oglplus::Texture();

    auto create_texture = [&](
      oglplus::Texture* tex, int width, int height, bool nearest,
      oglplus::enums::PixelDataInternalFormat i_format,
      oglplus::enums::PixelDataFormat         p_format) {

      oglplus::Texture::Active(0);
      ctx.gl.Bound(oglplus::Texture::Target::_2D, *tex)
        .Image2D(0, i_format, width, height,
          0, p_format, oglplus::PixelDataType::Float, nullptr)
        .MinFilter(nearest ? oglplus::TextureMinFilter::Nearest : oglplus::TextureMinFilter::Linear)
        .MagFilter(nearest ? oglplus::TextureMagFilter::Nearest : oglplus::TextureMagFilter::Linear)
        .WrapS(oglplus::TextureWrap::MirroredRepeat)
        .WrapT(oglplus::TextureWrap::MirroredRepeat);
    };

    create_texture(
      offscreen_color_, ctx.size.x(), ctx.size.y(), true,
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      offscreen_normal_, ctx.size.x(), ctx.size.y(), true,
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      offscreen_aux_1_, ctx.size.x(), ctx.size.y(), true,
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      offscreen_aux_2_, ctx.size.x(), ctx.size.y(), true,
      oglplus::PixelDataInternalFormat::Red,
      oglplus::PixelDataFormat::Red
    );
    create_texture(
      offscreen_light_, ctx.size.x(), ctx.size.y(), true,
      oglplus::PixelDataInternalFormat::RGBA,
      oglplus::PixelDataFormat::RGBA
    );

    // create framebuffer object -----------------------------------------------
    fbo_ = new oglplus::Framebuffer();
    fbo_->Bind(oglplus::Framebuffer::Target::Draw);

    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 0, *offscreen_color_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 1, *offscreen_normal_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 2, *offscreen_aux_1_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 3, *offscreen_aux_2_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 4, *offscreen_light_, 0
    );

    // create shaders ----------------------------------------------------------
    shader_ = new Shader(R"(
      // vertex shader ---------------------------------------------------------
      @include "fullscreen_quad_vertext_shader"
    )", R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      @include "gbuffer_input"
      @include "lbuffer_input"

      uniform bool debug;

      layout (location = 0) out vec4 fragColor;

      void main(void) {
        vec3  diffuse  = get_diffuse();
        vec3  light    = get_diffuse_light();
        vec3  specular = get_specular_light();
        float emit     = min(1.0, get_emit());

        fragColor      = vec4(emit * diffuse + (1 - emit) * (light*diffuse + specular), 1.0);

        if (debug) {
          fragColor = vec4(vec3(get_emit())  , 1.0);
        }
      }
    )");

    if (ShadingQuality() > 1) {
      post_processor_ = new PostProcessor(ctx);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_objects(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {

  ctx.gl.BlendFunc(
    oglplus::BlendFunction::SrcAlpha,
    oglplus::BlendFunction::OneMinusSrcAlpha
  );

  if (ShadingQuality() > 0) {
    if (!fbo_) {
      init(ctx);
    }

    fbo_->Bind(oglplus::Framebuffer::Target::Draw);

    oglplus::Context::ColorBuffer draw_buffs[4] =  {
      oglplus::FramebufferColorAttachment::_0,
      oglplus::FramebufferColorAttachment::_1,
      oglplus::FramebufferColorAttachment::_2,
      oglplus::FramebufferColorAttachment::_3
    };
    ctx.gl.DrawBuffers(draw_buffs);

    GLfloat clear0[3] = {0.f, 0.f, 0.f};
    ctx.gl.ClearColorBuffer(0, clear0);

    GLfloat clear1[3] = {0.5f, 0.5f, 0.f};
    ctx.gl.ClearColorBuffer(1, clear1);

    GLfloat clear2[3] = {1.f, 1.f, 1.f};
    ctx.gl.ClearColorBuffer(2, clear2);

    GLfloat clear3[1] = {0.0f};
    ctx.gl.ClearColorBuffer(3, clear3);

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

  if (ShadingQuality() > 0) {
    if (!fbo_) {
      init(ctx);
    }

    ctx.gl.BlendFunc(
      oglplus::BlendFunction::One,
      oglplus::BlendFunction::OneMinusSrcColor
    );

    ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_4);

    GLfloat clear[3] = {0.f, 0.f, 0.f};
    ctx.gl.ClearColorBuffer(0, clear);

    oglplus::Texture::Active(1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_color_);

    oglplus::Texture::Active(2);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_normal_);

    oglplus::Texture::Active(3);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_aux_1_);

    for (auto& light: scene->lights) {
      light.second->draw(ctx);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::composite(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  if (ShadingQuality() > 0) {
    if (!fbo_) {
      init(ctx);
    }

    ctx.gl.BlendFunc(
      oglplus::BlendFunction::SrcAlpha,
      oglplus::BlendFunction::OneMinusSrcAlpha
    );

    if (ShadingQuality() > 1) {
      ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_1);
    } else {
      oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);
      ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);
    }

    oglplus::Texture::Active(0);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_color_);

    oglplus::Texture::Active(1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_light_);

    oglplus::Texture::Active(2);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_aux_1_);

    shader_->use(ctx);
    shader_->set_uniform("g_buffer_diffuse", 0);
    shader_->set_uniform("g_buffer_light", 1);
    shader_->set_uniform("g_buffer_aux_1", 2);
    shader_->set_uniform("screen_size", ctx.size);
    shader_->set_uniform("debug", 0);

    Quad::instance()->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::post_process(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  if (ShadingQuality() > 1) {

    oglplus::Texture::Active(0);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_normal_);

    oglplus::Texture::Active(2);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_aux_2_);

    post_processor_->process(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_gui(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  for (auto& gui: scene->gui_elements) {
    gui.second->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::clean_up() {
  if(shader_)           delete shader_;             shader_           = nullptr;

  if(post_processor_)   delete post_processor_;     post_processor_   = nullptr;

  if(fbo_)              delete fbo_;                fbo_              = nullptr;
  if(offscreen_color_)  delete offscreen_color_;    offscreen_color_  = nullptr;
  if(offscreen_normal_) delete offscreen_normal_;   offscreen_normal_ = nullptr;
  if(offscreen_light_)  delete offscreen_light_;    offscreen_light_  = nullptr;
  if(offscreen_aux_1_)  delete offscreen_aux_1_;    offscreen_aux_1_  = nullptr;
  if(offscreen_aux_2_)  delete offscreen_aux_2_;    offscreen_aux_2_  = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

}
