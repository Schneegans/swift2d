////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/textures/DefaultTexture3D.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DefaultTexture3D::DefaultTexture3D()
  : texture_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

DefaultTexture3D::~DefaultTexture3D() {
  if (texture_) {
    delete texture_;
  }
}

////////////////////////////////////////////////////////////////////////////////

void DefaultTexture3D::bind(unsigned location) const {
  if (!texture_) {
    upload();
  }

  texture_->Active(location);
  ogl::Context::Bind(ose::_3D(), *texture_);
}

////////////////////////////////////////////////////////////////////////////////

void DefaultTexture3D::upload() const {

  texture_ = new oglplus::Texture();

  std::vector<unsigned char> data = {0, 0, 0, 0};

  ogl::Context::Bound(ose::_3D(), *texture_)
    .Image3D(0, oglplus::PixelDataInternalFormat::RGBA, 1, 1, 1,
             0, oglplus::PixelDataFormat::RGBA,
             oglplus::PixelDataType::UnsignedByte, &data.front());
}

////////////////////////////////////////////////////////////////////////////////

}
