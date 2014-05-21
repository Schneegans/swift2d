////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/resources/Texture.hpp>

#include <oglplus/images/png.hpp>
#include <oglplus/images/newton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Texture::Texture(std::string const& file_name)
  : texture_(nullptr)
  , file_name_(file_name)
  , needs_update_(true)
{}

////////////////////////////////////////////////////////////////////////////////

Texture::~Texture() {
  if (texture_) delete texture_;
}

////////////////////////////////////////////////////////////////////////////////

void Texture::load_from_file(std::string const& file_name) {
  file_name_ = file_name_;
  needs_update_ = true;
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

void Texture::unbind() const {

}

////////////////////////////////////////////////////////////////////////////////

void Texture::upload_to(RenderContext const& context) const {

  if (texture_) {
    delete texture_;
  }

  texture_ = new oglplus::Texture();

  auto png  = oglplus::images::PNGImage(file_name_.c_str(), true, true);

  context.gl.Bound(oglplus::smart_enums::_2D(), *texture_)
    .Image2D(png)
    .MinFilter(oglplus::smart_enums::Linear())
    .MagFilter(oglplus::smart_enums::Linear())
    .Anisotropy(2.0f)
    .WrapS(oglplus::smart_enums::ClampToEdge())
    .WrapT(oglplus::smart_enums::ClampToEdge());

  needs_update_ = false;
}

////////////////////////////////////////////////////////////////////////////////

}

