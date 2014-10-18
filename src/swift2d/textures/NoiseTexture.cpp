////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/textures/NoiseTexture.hpp>

#include <oglplus/images/random.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

NoiseTexture::NoiseTexture()
  : texture_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

NoiseTexture::~NoiseTexture() {
  delete texture_;
}

////////////////////////////////////////////////////////////////////////////////

void NoiseTexture::bind(RenderContext const& context, unsigned location) const {
  if (!texture_) {
    upload_to(context);
  }

  texture_->Active(location);
  context.gl.Bind(ose::_2D(), *texture_);
}

////////////////////////////////////////////////////////////////////////////////

void NoiseTexture::upload_to(RenderContext const& context) const {

  texture_ = new oglplus::Texture();

  context.gl.Bound(ose::_2D(), *texture_)
    .Image2D(oglplus::images::RandomRGBUByte(256, 256))
    .MaxLevel(0)
    .MinFilter(ose::Linear())
    .MagFilter(ose::Linear())
    .WrapS(ose::Repeat())
    .WrapT(ose::Repeat());
}

////////////////////////////////////////////////////////////////////////////////

}
