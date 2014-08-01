////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/textures/DefaultAnimatedTexture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DefaultAnimatedTexture::DefaultAnimatedTexture()
  : texture_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

DefaultAnimatedTexture::~DefaultAnimatedTexture() {
  delete texture_;
}

////////////////////////////////////////////////////////////////////////////////

void DefaultAnimatedTexture::bind(RenderContext const& context, unsigned location) const {
  if (!texture_) {
    upload_to(context);
  }

  texture_->Active(location);
  context.gl.Bind(ose::_3D(), *texture_);
}

////////////////////////////////////////////////////////////////////////////////

void DefaultAnimatedTexture::upload_to(RenderContext const& context) const {

  texture_ = new oglplus::Texture();

  std::vector<unsigned char> data = {0, 0, 0, 0};

  context.gl.Bound(ose::_3D(), *texture_)
    .Image3D(0, oglplus::PixelDataInternalFormat::RGBA, 1, 1, 1,
             0, oglplus::PixelDataFormat::RGBA,
             oglplus::PixelDataType::UnsignedByte, &data.front());
}

////////////////////////////////////////////////////////////////////////////////

}
