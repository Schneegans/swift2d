////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/textures/Texture.hpp>

#include <swift2d/application/Application.hpp>
#include <oglplus/images/png.hpp>
#include <oglplus/images/newton.hpp>
#include <stb_image/stb_image.h>
#include <istream>
#include <streambuf>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Texture::Texture()
  : FileName("")
  , texture_(nullptr)
  , needs_update_(true) {

  FileName.on_change().connect([&](std::string const& path){
    needs_update_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

Texture::Texture(std::string const& file_name)
  : FileName(file_name)
  , texture_(nullptr)
  , needs_update_(true) {

  FileName.on_change().connect([&](std::string const& path){
    needs_update_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

Texture::~Texture() {
  if (texture_) delete texture_;
}

////////////////////////////////////////////////////////////////////////////////

void Texture::bind(RenderContext const& context, unsigned location) const {
  if (needs_update_) {
    upload_to(context);
  }

  texture_->Active(location);
  context.gl.Bind(ose::_2D(), *texture_);
}

////////////////////////////////////////////////////////////////////////////////

void Texture::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("FileName", FileName);
}

////////////////////////////////////////////////////////////////////////////////

void Texture::upload_to(RenderContext const& context) const {

  needs_update_ = false;

  if (texture_) {
    delete texture_;
  }

  int width(0), height(0), channels(0);
  unsigned char* data(load_texture_data(FileName(), &width, &height, &channels));

  if (data) {
    auto internal_format(channels > 3 ? ogl::InternalFormat::RGBA : ogl::InternalFormat::RGB);
    auto format(channels > 3 ? ogl::Format::RGBA : ogl::Format::RGB);

    texture_ = new ogl::Texture();

    context.gl.Bound(ose::_2D(), *texture_)
      .Image2D(0, internal_format, width, height, 0, format,
               ogl::DataType::UnsignedByte, data)
      .GenerateMipmap()
      .MinFilter(ose::LinearMipmapLinear())
      .MagFilter(ose::Linear())
      .WrapS(ose::Repeat())
      .WrapT(ose::Repeat());

  } else {
    Logger::LOG_ERROR << "Failed to load texture \"" << FileName() << "\"!" << std::endl;
  }

  free_texture_data(data);
}

////////////////////////////////////////////////////////////////////////////////

unsigned char* Texture::load_texture_data(std::string f, int* w, int* h, int* c) const {
  if (f[0] != '/') {
    f = Application::instance()->make_absolute(f);
  }

  unsigned char* data(stbi_load(f.c_str(), w, h, c, STBI_default));

  if (data && *w && *h) {
    return data;
  }

  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void Texture::free_texture_data(unsigned char* data) const {
  stbi_image_free(data);
}

////////////////////////////////////////////////////////////////////////////////

}

