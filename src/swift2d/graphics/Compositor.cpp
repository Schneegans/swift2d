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

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Compositor::Compositor()
  : EnableDynamicLighting(false)
  , vs_(nullptr)
  , fs_(nullptr)
  , prog_(nullptr)
  , fbo_(nullptr)
  , offscreen_color_(nullptr)
  , offscreen_normal_(nullptr)
  , offscreen_light_(nullptr)
  , offscreen_emit_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

Compositor::~Compositor() {
  clean_up();
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::init(RenderContext const& ctx) {

  clean_up();

  if (EnableDynamicLighting()) {

    // create textures for G-Buffer and L-Buffer ---------------------------------
    offscreen_color_  = new oglplus::Texture();
    offscreen_normal_ = new oglplus::Texture();
    offscreen_light_  = new oglplus::Texture();
    offscreen_emit_   = new oglplus::Texture();

    auto create_texture = [&](oglplus::Texture* tex, int loc,
                              oglplus::enums::PixelDataInternalFormat i_format,
                              oglplus::enums::PixelDataFormat p_format){

      oglplus::Texture::Active(loc);
      ctx.gl.Bound(oglplus::Texture::Target::_2D, *tex)
        .MinFilter(oglplus::TextureMinFilter::Nearest)
        .MagFilter(oglplus::TextureMagFilter::Nearest)
        .WrapS(oglplus::TextureWrap::ClampToEdge)
        .WrapT(oglplus::TextureWrap::ClampToEdge)
        .Image2D(0, i_format, ctx.size.x(), ctx.size.y(),
          0, p_format, oglplus::PixelDataType::Float, nullptr
        );
    };

    create_texture(offscreen_color_,  0, oglplus::PixelDataInternalFormat::RGB,
                   oglplus::PixelDataFormat::RGB);
    create_texture(offscreen_normal_, 1, oglplus::PixelDataInternalFormat::RGB,
                   oglplus::PixelDataFormat::RGB);
    create_texture(offscreen_emit_,   2, oglplus::PixelDataInternalFormat::RGB,
                   oglplus::PixelDataFormat::RGB);
    create_texture(offscreen_light_,  3, oglplus::PixelDataInternalFormat::RGB,
                   oglplus::PixelDataFormat::RGB);


    // create framebuffer object -------------------------------------------------
    fbo_ = new oglplus::Framebuffer();
    fbo_->Bind(oglplus::Framebuffer::Target::Draw);
    oglplus::Framebuffer::AttachColorTexture(oglplus::Framebuffer::Target::Draw,
                                             0, *offscreen_color_, 0);
    oglplus::Framebuffer::AttachColorTexture(oglplus::Framebuffer::Target::Draw,
                                             1, *offscreen_normal_, 0);
    oglplus::Framebuffer::AttachColorTexture(oglplus::Framebuffer::Target::Draw,
                                             2, *offscreen_emit_, 0);
    oglplus::Framebuffer::AttachColorTexture(oglplus::Framebuffer::Target::Draw,
                                             3, *offscreen_light_, 0);


    // create shaders ------------------------------------------------------------
    // set the vertex shader source
    std::stringstream vs_source(
      "#version 330\n"
      "layout(location=0) in vec2 position;"
      "uniform mat3 transform;"
      "out vec2 tex_coords;"
      "void main(void){"
      "  tex_coords = position*0.5 + 0.5;"
      "  gl_Position = vec4(position, 0.0, 1.0);"
      "}"
    );
    vs_ = new oglplus::Shader(oglplus::ShaderType::Vertex);
    vs_->Source(oglplus::GLSLSource::FromStream(vs_source));
    try {
      vs_->Compile();
    } catch (oglplus::CompileError& e) {
      LOG_ERROR << "Failed to compile Vertex Shader!" << std::endl;
      LOG_ERROR << e.Log() << std::endl;
    }

    // set the fragment shader source
    std::stringstream fs_source(
      "#version 330\n"
      "in vec2 tex_coords;"
      "uniform sampler2D g_buffer_diffuse;"
      "uniform sampler2D g_buffer_light;"
      "uniform sampler2D g_buffer_emit;"
      "uniform bool      debug;"
      ""
      "layout (location = 0) out vec3 fragColor;"
      ""
      "void main(void){"
      "  vec3 diffuse  = texture2D(g_buffer_diffuse, tex_coords).rgb;"
      "  vec3 light    = texture2D(g_buffer_light, tex_coords).rgb;"
      "  vec3 emit     = texture2D(g_buffer_emit, tex_coords).rgb;"
      "  fragColor     = emit.r * diffuse + (1 - emit.r) * light * diffuse;"
      "  if (debug) {"
      "    fragColor   = texture2D(g_buffer_diffuse, tex_coords).rgb;"
      "  }"
      "}"
    );

    fs_ = new oglplus::Shader(oglplus::ShaderType::Fragment);
    fs_->Source(oglplus::GLSLSource::FromStream(fs_source));
    try {
      fs_->Compile();
    } catch (oglplus::CompileError& e) {
      LOG_ERROR << "Failed to compile Fragment Shader!" << std::endl;
      LOG_ERROR << e.Log() << std::endl;
    }

    // attach the shaders to the program
    prog_ = new oglplus::Program();
    prog_->AttachShader(*vs_);
    prog_->AttachShader(*fs_);

    // link it
    try {
      prog_->Link();
    } catch (oglplus::LinkError& e) {
      LOG_ERROR << e.Log() << std::endl;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_objects(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  ctx.gl.BlendFunc(
    oglplus::BlendFunction::SrcAlpha,
    oglplus::BlendFunction::OneMinusSrcAlpha
  );

  if (EnableDynamicLighting()) {
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

    GLfloat clear[3] = {0.5f, 0.5f, 0.f};
    ctx.gl.ClearColorBuffer(1, clear);

  } else {
    oglplus::DefaultFramebuffer::Bind(oglplus::Framebuffer::Target::Draw);
    ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);
  }

  for (auto& object: scene->objects) {
    object.second->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_lights(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  if (EnableDynamicLighting()) {
    if (!fbo_) {
      init(ctx);
    }

    ctx.gl.BlendFunc(
      oglplus::BlendFunction::One,
      oglplus::BlendFunction::One
    );

    ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_3);

    GLfloat clear[3] = {0.f, 0.f, 0.f};
    ctx.gl.ClearColorBuffer(0, clear);

    oglplus::Texture::Active(1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_color_);

    oglplus::Texture::Active(2);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_normal_);

    for (auto& light: scene->lights) {
      light.second->draw(ctx);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::composite(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  if (EnableDynamicLighting()) {
    if (!fbo_) {
      init(ctx);
    }

    oglplus::DefaultFramebuffer::Bind(oglplus::Framebuffer::Target::Draw);
    ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);

    oglplus::Texture::Active(0);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_color_);

    oglplus::Texture::Active(1);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_light_);

    oglplus::Texture::Active(2);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_emit_);

    oglplus::Texture::Active(3);
    ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_normal_);

    prog_->Use();

    (*prog_/"g_buffer_diffuse") = 0;
    (*prog_/"g_buffer_light") = 1;
    (*prog_/"g_buffer_emit") = 2;

    (*prog_/"debug") = 0;

    Quad::instance()->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::clean_up() {
  if(vs_)               delete vs_;                 vs_ = nullptr;
  if(fs_)               delete fs_;                 fs_ = nullptr;
  if(prog_)             delete prog_;               prog_ = nullptr;
  if(fbo_)              delete fbo_;                fbo_ = nullptr;
  if(offscreen_color_)  delete offscreen_color_;    offscreen_color_ = nullptr;
  if(offscreen_normal_) delete offscreen_normal_;   offscreen_normal_ = nullptr;
  if(offscreen_light_)  delete offscreen_light_;    offscreen_light_ = nullptr;
  if(offscreen_emit_)   delete offscreen_emit_;     offscreen_emit_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

}
