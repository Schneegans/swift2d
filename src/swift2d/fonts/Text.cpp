////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/fonts/Text.hpp>
#include <swift2d/graphics/RenderContext.hpp>

#include <oglplus/text/pango_cairo.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Text::Text()
  : text_update_(true)
  , font_update_(true) {

  Content.on_change().connect([&](std::string const& val){
    text_update_ = true;
  });

  Size.on_change().connect([&](int val){
    text_update_ = true;
  });

  Font.on_change().connect([&](std::string const& val){
    text_update_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

LayoutPtr const& Text::get_layout() {
  if (font_update_) {
    layout_ = FontRenderer::instance()->make_layout(Font(), Size(), 128);
    font_update_ = false;
  }

  if (text_update_) {
    layout_->Set(Content());
    text_update_ = false;
  }

  return layout_;
}

////////////////////////////////////////////////////////////////////////////////

}

