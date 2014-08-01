////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/textures/DefaultAnimatedTexture.hpp>

#include <oglplus/images/random.hpp>

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

  context.gl.Bound(ose::_3D(), *texture_)
    .Image3D(oglplus::images::RandomRGBUByte(4, 4, 4))
    .MinFilter(ose::Linear())
    .MagFilter(ose::Linear())
    .WrapS(ose::Repeat())
    .WrapT(ose::Repeat())
    .WrapR(ose::Repeat());
}

////////////////////////////////////////////////////////////////////////////////

}
