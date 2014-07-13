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
  context.gl.Bind(oglplus::smart_enums::_2D(), *texture_);
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

  std::string file(FileName.get());

  if (file[0] != '/') {
    file = Application::instance()->make_absolute(file);
  }

  int width(0), height(0), channels(0);
  unsigned char* data(stbi_load(file.c_str(), &width, &height,
                                &channels, STBI_default));

  bool success(data && width && height);

  if (success) {
    auto internal_format(channels > 3 ? oglplus::InternalFormat::RGBA : oglplus::InternalFormat::RGB);
    auto format(channels > 3 ? oglplus::Format::RGBA : oglplus::Format::RGB);

    texture_ = new oglplus::Texture();

    context.gl.Bound(oglplus::smart_enums::_2D(), *texture_)
      .Image2D(0, internal_format, width, height, 0, format,
               oglplus::DataType::UnsignedByte, data)
      .GenerateMipmap()
      .MinFilter(oglplus::smart_enums::LinearMipmapLinear())
      .MagFilter(oglplus::smart_enums::Linear())
      .WrapS(oglplus::smart_enums::Repeat())
      .WrapT(oglplus::smart_enums::Repeat());
  }

  stbi_image_free(data);
}

////////////////////////////////////////////////////////////////////////////////

}

