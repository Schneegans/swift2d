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
  : shading_quality_  (shading_quality)
  , shader_           (nullptr)
  , fbo_              ()
  , offscreen_color_  ()
  , offscreen_normal_ ()
  , offscreen_light_  ()
  , offscreen_aux_1_  ()
  , offscreen_aux_2_  ()
  , post_processor_   (nullptr) {

  if (shading_quality_ > 0) {

    // create textures for G-Buffer and L-Buffer -------------------------------
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

    create_texture(
      offscreen_color_, ctx.size.x(), ctx.size.y(),
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      offscreen_normal_, ctx.size.x(), ctx.size.y(),
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      offscreen_aux_1_, ctx.size.x(), ctx.size.y(),
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      offscreen_aux_2_, ctx.size.x(), ctx.size.y(),
      oglplus::PixelDataInternalFormat::Red,
      oglplus::PixelDataFormat::Red
    );
    create_texture(
      offscreen_light_, ctx.size.x(), ctx.size.y(),
      oglplus::PixelDataInternalFormat::RGBA,
      oglplus::PixelDataFormat::RGBA
    );

    // create framebuffer object -----------------------------------------------
    fbo_.Bind(oglplus::Framebuffer::Target::Draw);

    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 0, offscreen_color_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 1, offscreen_normal_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 2, offscreen_aux_1_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 3, offscreen_aux_2_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 4, offscreen_light_, 0
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
          fragColor = vec4(texture2D(g_buffer_light, gl_FragCoord.xy/screen_size).rgb , 1.0);
        }
      }
    )");

    if (shading_quality_ > 1) {
      post_processor_ = new PostProcessor(ctx);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

Compositor::~Compositor() {
  if(shader_)         delete shader_;
  if(post_processor_) delete post_processor_;
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_objects(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {

  ctx.gl.BlendFunc(
    oglplus::BlendFunction::SrcAlpha,
    oglplus::BlendFunction::OneMinusSrcAlpha
  );

  if (shading_quality_ > 0) {

    fbo_.Bind(oglplus::Framebuffer::Target::Draw);

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

  if (shading_quality_ > 0) {
    ctx.gl.BlendFunc(
      oglplus::BlendFunction::One,
      oglplus::BlendFunction::OneMinusSrcColor
    );

    if (shading_quality_ > 1) {
      ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_4);
    } else {
      oglplus::DefaultFramebuffer().Bind(oglplus::Framebuffer::Target::Draw);
      ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);
    }

    GLfloat clear[3] = {0.f, 0.f, 0.f};
    ctx.gl.ClearColorBuffer(0, clear);

    oglplus::Texture::Active(1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), offscreen_color_);

    oglplus::Texture::Active(2);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), offscreen_normal_);

    oglplus::Texture::Active(3);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), offscreen_aux_1_);

    for (auto& light: scene->lights) {
      light.second->draw(ctx);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::post_process(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  if (shading_quality_ > 1) {

    oglplus::Texture::Active(0);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), offscreen_light_);

    oglplus::Texture::Active(2);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), offscreen_aux_2_);

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

}
