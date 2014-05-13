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

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Compositor::Compositor()
  : vs_(nullptr)
  , fs_(nullptr)
  , prog_(nullptr)
  , fbo_(nullptr)
  , offscreen_color_(nullptr)
  , offscreen_normal_(nullptr)
  , offscreen_light_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

Compositor::~Compositor() {
  clean_up();
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::init(RenderContext const& ctx) {

  clean_up();

  fullscreen_quad_ = ScreenQuadResource::create();

  // create textures for G-Buffer and L-Buffer ---------------------------------
  offscreen_color_  = new oglplus::Texture();
  offscreen_normal_ = new oglplus::Texture();
  offscreen_light_  = new oglplus::Texture();

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

  create_texture(offscreen_color_,  2, oglplus::PixelDataInternalFormat::RGBA,
                 oglplus::PixelDataFormat::RGBA);
  create_texture(offscreen_normal_, 3, oglplus::PixelDataInternalFormat::RGBA,
                 oglplus::PixelDataFormat::RGBA);
  create_texture(offscreen_light_,  4, oglplus::PixelDataInternalFormat::RGBA,
                 oglplus::PixelDataFormat::RGBA);


  // create framebuffer object -------------------------------------------------
  fbo_ = new oglplus::Framebuffer();
  fbo_->Bind(oglplus::Framebuffer::Target::Draw);
  oglplus::Framebuffer::AttachColorTexture(oglplus::Framebuffer::Target::Draw,
                                           0, *offscreen_color_, 0);
  oglplus::Framebuffer::AttachColorTexture(oglplus::Framebuffer::Target::Draw,
                                           1, *offscreen_normal_, 0);
  oglplus::Framebuffer::AttachColorTexture(oglplus::Framebuffer::Target::Draw,
                                           2, *offscreen_light_, 0);


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
    "uniform sampler2D diffuse;"
    "uniform sampler2D light;"
    ""
    "layout (location = 0) out vec4 fragColor;"
    ""
    "void main(void){"
    "  vec4 light_color = texture2D(light, tex_coords);"
    "  fragColor   = texture2D(diffuse, tex_coords) + light_color;"
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

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_objects(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  ctx.gl.BlendFunc(
    oglplus::BlendFunction::SrcAlpha,
    oglplus::BlendFunction::OneMinusSrcAlpha
  );

  // oglplus::DefaultFramebuffer::Bind(oglplus::Framebuffer::Target::Draw);
  // ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);

  fbo_->Bind(oglplus::Framebuffer::Target::Draw);

  oglplus::Context::ColorBuffer draw_buffs[2] =  {
    oglplus::FramebufferColorAttachment::_0,
    oglplus::FramebufferColorAttachment::_1
  };
  ctx.gl.DrawBuffers(draw_buffs);

  for (auto& object: scene->objects) {
    object.second->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_lights(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  ctx.gl.BlendFunc(
    oglplus::BlendFunction::One,
    oglplus::BlendFunction::One
  );

  ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_2);

  GLfloat clear[4] = {0.f, 0.f, 0.f, 0.f};
  ctx.gl.ClearColorBuffer(0, clear);

  for (auto& light: scene->lights) {
    light.second->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::composite(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  oglplus::DefaultFramebuffer::Bind(oglplus::Framebuffer::Target::Draw);
  ctx.gl.DrawBuffer(oglplus::ColorBuffer::BackLeft);

  oglplus::Texture::Active(0);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_color_);

  oglplus::Texture::Active(1);
  ctx.gl.Bind(oglplus::smart_enums::_2D(), *offscreen_light_);

  prog_->Use();

  (*prog_/"diffuse") = 0;
  (*prog_/"light") = 1;

  fullscreen_quad_->draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::clean_up() {
  if(vs_)               delete vs_;
  if(fs_)               delete fs_;
  if(prog_)             delete prog_;
  if(fbo_)              delete fbo_;
  if(offscreen_color_)  delete offscreen_color_;
  if(offscreen_normal_) delete offscreen_normal_;
  if(offscreen_light_)  delete offscreen_light_;
}

////////////////////////////////////////////////////////////////////////////////

}
