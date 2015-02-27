////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
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

void NoiseTexture::bind(RenderContext const& ctx, unsigned location) const {
  if (!texture_) {
    upload_to(ctx);
  }

  texture_->Active(location);
  ogl::Context::Bind(ose::_2D(), *texture_);
}

////////////////////////////////////////////////////////////////////////////////

void NoiseTexture::upload_to(RenderContext const& ctx) const {

  texture_ = new oglplus::Texture();

  ogl::Context::Bound(ose::_2D(), *texture_)
    .Image2D(oglplus::images::RandomRGBUByte(256, 256))
    .MaxLevel(0)
    .MinFilter(ose::Linear())
    .MagFilter(ose::Linear())
    .WrapS(ose::Repeat())
    .WrapT(ose::Repeat());
}

////////////////////////////////////////////////////////////////////////////////

}
