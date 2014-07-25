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
  context.gl.Bind(oglplus::smart_enums::_2D(), *texture_);
}

////////////////////////////////////////////////////////////////////////////////

void NoiseTexture::upload_to(RenderContext const& context) const {

  texture_ = new oglplus::Texture();

  context.gl.Bound(oglplus::smart_enums::_2D(), *texture_)
    .Image2D(oglplus::images::RandomRGBUByte(256, 256))
    .MinFilter(oglplus::smart_enums::Linear())
    .MagFilter(oglplus::smart_enums::Linear())
    .WrapS(oglplus::smart_enums::Repeat())
    .WrapT(oglplus::smart_enums::Repeat());
}

////////////////////////////////////////////////////////////////////////////////

}
