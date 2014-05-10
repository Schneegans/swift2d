////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/resources/TextureResource.hpp>

#include <oglplus/images/png.hpp>
#include <oglplus/images/newton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TextureResource::TextureResource(std::string const& file_name)
  : file_name_(file_name)
  , needs_update_(true)
{}

////////////////////////////////////////////////////////////////////////////////

void TextureResource::load_from_file(std::string const& file_name) {
  file_name_ = file_name_;
  needs_update_ = true;
}

////////////////////////////////////////////////////////////////////////////////

void TextureResource::bind(RenderContext const& context, unsigned location) const {
  if (needs_update_) {

    auto png  = oglplus::images::PNGImage(file_name_.c_str(), true, true);

    context.gl.Bound(oglplus::smart_enums::_2D(), texture_)
      .Image2D(png)
      .MinFilter(oglplus::smart_enums::Linear())
      .MagFilter(oglplus::smart_enums::Linear())
      .Anisotropy(2.0f)
      .WrapS(oglplus::smart_enums::Repeat())
      .WrapT(oglplus::smart_enums::Repeat());
  }

  context.gl.Bound(oglplus::smart_enums::_2D(), texture_);
}

////////////////////////////////////////////////////////////////////////////////

void TextureResource::unbind() const {

}

////////////////////////////////////////////////////////////////////////////////

void TextureResource::upload_to(RenderContext const& context) const {

}

////////////////////////////////////////////////////////////////////////////////

}

