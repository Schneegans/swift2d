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
  , post_fx_shader_   (nullptr)
  , fbo_              (nullptr)
  , offscreen_color_  (nullptr)
  , offscreen_normal_ (nullptr)
  , offscreen_light_  (nullptr)
  , offscreen_aux_    (nullptr)
  , glow_threshold_shader_ (nullptr)
  , glow_shader_      (nullptr)
  , glow_fbo_         (nullptr)
  , glow_ping_        (nullptr)
  , glow_pong_        (nullptr) {}

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
    offscreen_aux_    = new oglplus::Texture();

    auto create_texture = [&](
      oglplus::Texture* tex, int width, int height, bool nearest,
      oglplus::enums::PixelDataInternalFormat i_format,
      oglplus::enums::PixelDataFormat         p_format) {

      oglplus::Texture::Active(0);
      ctx.gl.Bound(oglplus::Texture::Target::_2D, *tex)
        .MinFilter(nearest ? oglplus::TextureMinFilter::Nearest : oglplus::TextureMinFilter::Linear)
        .MagFilter(nearest ? oglplus::TextureMagFilter::Nearest : oglplus::TextureMagFilter::Linear)
        .WrapS(oglplus::TextureWrap::MirroredRepeat)
        .WrapT(oglplus::TextureWrap::MirroredRepeat)
        .Image2D(0, i_format, width, height,
          0, p_format, oglplus::PixelDataType::Float, nullptr
        );
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
      offscreen_aux_, ctx.size.x(), ctx.size.y(), true,
      oglplus::PixelDataInternalFormat::RGB,
      oglplus::PixelDataFormat::RGB
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
      oglplus::Framebuffer::Target::Draw, 2, *offscreen_aux_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 3, *offscreen_light_, 0
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
          fragColor = texture2D(g_buffer_aux, gl_FragCoord.xy/screen_size);
        }
      }
    )");

    if (ShadingQuality() > 1) {

      glow_ping_ = new oglplus::Texture();
      glow_pong_ = new oglplus::Texture();

      create_texture(
        glow_ping_, ctx.size.x()/3, ctx.size.y()/3, false,
        oglplus::PixelDataInternalFormat::RGB,
        oglplus::PixelDataFormat::RGB
      );

      create_texture(
        glow_pong_, ctx.size.x()/3, ctx.size.y()/3, false,
        oglplus::PixelDataInternalFormat::RGB,
        oglplus::PixelDataFormat::RGB
      );

      // create framebuffer object -----------------------------------------------
      glow_fbo_ = new oglplus::Framebuffer();
      glow_fbo_->Bind(oglplus::Framebuffer::Target::Draw);
      oglplus::Framebuffer::AttachColorTexture(
        oglplus::Framebuffer::Target::Draw, 0, *glow_ping_, 0
      );
      oglplus::Framebuffer::AttachColorTexture(
        oglplus::Framebuffer::Target::Draw, 1, *glow_pong_, 0
      );


      // create shaders ----------------------------------------------------------
      post_fx_shader_ = new Shader(R"(
        // vertex shader ---------------------------------------------------------
        @include "fullscreen_quad_vertext_shader"
      )", R"(
        // fragment shader -------------------------------------------------------
        @include "version"

        uniform sampler2D g_buffer_shaded;
        uniform sampler2D g_glow;
        uniform ivec2     screen_size;

        layout (location = 0) out vec4 fragColor;

        void main(void) {
          vec3  diffuse  = texture2D(g_buffer_shaded, gl_FragCoord.xy/screen_size).rgb;
          vec3  glow     = texture2D(g_glow, gl_FragCoord.xy/screen_size).rgb;
          fragColor      = vec4(diffuse + glow*5.0, 1.0);
        }
      )");

      glow_threshold_shader_ = new Shader(R"(
        // vertex shader ---------------------------------------------------------
        @include "fullscreen_quad_vertext_shader"
      )", R"(
        // fragment shader -------------------------------------------------------
        @include "version"

        @include "gbuffer_input"

        layout (location = 0) out vec4 fragColor;

        void main(void) {
          float fac = max(0, get_emit() - 1.0);
          fragColor = vec4(get_diffuse() * fac, 1.0);
        }
      )");

      glow_shader_ = new Shader(R"(
        // vertex shader ---------------------------------------------------------
        @include "fullscreen_quad_vertext_shader"
      )", R"(
        // fragment shader -------------------------------------------------------
        @include "version"

        uniform sampler2D g_glow;
        uniform ivec2     screen_size;
        uniform float     flare_length;

        layout (location = 0) out vec4 fragColor;

        void main(void) {

          const float samples = 10.0;

          vec2 texcoords = gl_FragCoord.xy/screen_size;
          vec2 step = vec2(flare_length / samples, 0);
          vec3 base = texture2D(g_glow, texcoords).rgb;
          vec3 col = vec3(0.0);

          for (float i = 0.0; i < samples; i += 1.0) {
            col += texture2D(g_glow, texcoords + (i-samples/2 + 0.5)*step).rgb;
          }

          fragColor = vec4(col / samples + 0.2*base, 1.0);
        }
      )");
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

    oglplus::Context::ColorBuffer draw_buffs[3] =  {
      oglplus::FramebufferColorAttachment::_0,
      oglplus::FramebufferColorAttachment::_1,
      oglplus::FramebufferColorAttachment::_2
    };
    ctx.gl.DrawBuffers(draw_buffs);

    GLfloat clear1[3] = {0.5f, 0.5f, 0.f};
    ctx.gl.ClearColorBuffer(1, clear1);

    GLfloat clear2[3] = {0.5f, 1.f, 1.f};
    ctx.gl.ClearColorBuffer(2, clear2);

  } else {
    oglplus::DefaultFramebuffer::Bind(oglplus::Framebuffer::Target::Draw);
    ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);
  }

  ctx.gl.Clear().ColorBuffer();

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

    ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_3);

    GLfloat clear[3] = {0.f, 0.f, 0.f};
    ctx.gl.ClearColorBuffer(0, clear);

    oglplus::Texture::Active(1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_color_);

    oglplus::Texture::Active(2);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_normal_);

    oglplus::Texture::Active(3);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_aux_);

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
      oglplus::DefaultFramebuffer::Bind(oglplus::Framebuffer::Target::Draw);
      ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);
    }

    oglplus::Texture::Active(0);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_color_);

    oglplus::Texture::Active(1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_light_);

    oglplus::Texture::Active(2);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_aux_);

    shader_->use(ctx);
    shader_->set_uniform("g_buffer_diffuse", 0);
    shader_->set_uniform("g_buffer_light", 1);
    shader_->set_uniform("g_buffer_aux", 2);
    shader_->set_uniform("screen_size", ctx.size);
    shader_->set_uniform("debug", 0);

    Quad::instance()->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::post_process(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  if (ShadingQuality() > 1) {
    if (!fbo_) {
      init(ctx);
    }

    oglplus::Texture::Active(0);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_normal_);

    oglplus::Texture::Active(2);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_aux_);

    ctx.gl.Disable(oglplus::Capability::Blend);


    // glow thresholding
    glow_fbo_->Bind(oglplus::Framebuffer::Target::Draw);

    ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);

    glow_threshold_shader_->use(ctx);
    glow_threshold_shader_->set_uniform("g_buffer_diffuse", 0);
    glow_threshold_shader_->set_uniform("g_buffer_aux", 2);
    glow_threshold_shader_->set_uniform("screen_size", ctx.size/3);

    Quad::instance()->draw(ctx);

    // glow blurring
    glow_shader_->use(ctx);
    glow_shader_->set_uniform("screen_size", ctx.size/3);
    oglplus::Texture::Active(1);
    glow_shader_->set_uniform("g_glow", 1);

    // pass 1
    ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *glow_ping_);
    glow_shader_->set_uniform("flare_length", 0.05f);
    Quad::instance()->draw(ctx);

    // pass 2
    ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *glow_pong_);
    glow_shader_->set_uniform("flare_length", 0.09f);
    Quad::instance()->draw(ctx);

    // pass 3
    ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *glow_ping_);
    glow_shader_->set_uniform("flare_length", 0.29f);
    Quad::instance()->draw(ctx);

    // pass 4
    ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *glow_pong_);
    glow_shader_->set_uniform("flare_length", 0.5f);
    Quad::instance()->draw(ctx);




    oglplus::DefaultFramebuffer::Bind(oglplus::Framebuffer::Target::Draw);
    ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);

    oglplus::Texture::Active(1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *glow_ping_);

    post_fx_shader_->use(ctx);
    post_fx_shader_->set_uniform("g_buffer_shaded", 0);
    post_fx_shader_->set_uniform("g_glow", 1);
    post_fx_shader_->set_uniform("screen_size", ctx.size);

    Quad::instance()->draw(ctx);

    ctx.gl.Enable(oglplus::Capability::Blend);
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
  if(post_fx_shader_)   delete post_fx_shader_;     post_fx_shader_   = nullptr;

  if(fbo_)              delete fbo_;                fbo_              = nullptr;
  if(offscreen_color_)  delete offscreen_color_;    offscreen_color_  = nullptr;
  if(offscreen_normal_) delete offscreen_normal_;   offscreen_normal_ = nullptr;
  if(offscreen_light_)  delete offscreen_light_;    offscreen_light_  = nullptr;
  if(offscreen_aux_)    delete offscreen_aux_;      offscreen_aux_    = nullptr;

  if(glow_fbo_)         delete glow_fbo_;           glow_fbo_         = nullptr;
  if(glow_ping_)        delete glow_ping_;          glow_ping_        = nullptr;
  if(glow_pong_)        delete glow_pong_;          glow_pong_        = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

}
