////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/textures/DefaultTexture.hpp>

#include <oglplus/images/random.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DefaultTexture::DefaultTexture()
  : texture_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

DefaultTexture::~DefaultTexture() {
  delete texture_;
}

////////////////////////////////////////////////////////////////////////////////

void DefaultTexture::bind(RenderContext const& context, unsigned location) const {
  if (!texture_) {
    upload_to(context);
  }

  texture_->Active(location);
  context.gl.Bind(ose::_2D(), *texture_);
}

////////////////////////////////////////////////////////////////////////////////

void DefaultTexture::upload_to(RenderContext const& context) const {

  texture_ = new oglplus::Texture();

  context.gl.Bound(ose::_2D(), *texture_)
    .Image2D(oglplus::images::RandomRGBUByte(4, 4))
    .MinFilter(ose::Linear())
    .MagFilter(ose::Linear())
    .WrapS(ose::Repeat())
    .WrapT(ose::Repeat());
}

////////////////////////////////////////////////////////////////////////////////

}
