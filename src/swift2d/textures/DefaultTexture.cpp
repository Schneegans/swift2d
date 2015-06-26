////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/textures/DefaultTexture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DefaultTexture::DefaultTexture()
  : texture_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

DefaultTexture::~DefaultTexture() {
  if (texture_) {
    delete texture_;
  }
}

////////////////////////////////////////////////////////////////////////////////

unsigned DefaultTexture::bind(unsigned location) const {
  if (!texture_) {
    upload();
  }

  texture_->Active(location);
  ogl::Context::Bind(ose::_2D(), *texture_);

  return location;
}

////////////////////////////////////////////////////////////////////////////////

void DefaultTexture::upload() const {

  texture_ = new oglplus::Texture();

  std::vector<unsigned char> data = {0, 0, 0, 0};

  ogl::Context::Bound(ose::_2D(), *texture_)
    .Image2D(0, oglplus::PixelDataInternalFormat::RGBA, 1, 1,
             0, oglplus::PixelDataFormat::RGBA,
             oglplus::PixelDataType::UnsignedByte, &data.front())
    .MaxLevel(0)
    .MinFilter(ose::Linear())
    .MagFilter(ose::Linear())
    .WrapS(ose::Repeat())
    .WrapT(ose::Repeat());
}

////////////////////////////////////////////////////////////////////////////////

}
