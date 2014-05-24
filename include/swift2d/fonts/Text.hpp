////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_FONT_HPP
#define SWIFT2D_FONT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/properties.hpp>
#include <swift2d/fonts/FontRenderer.hpp>

#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// shared pointer type definition ----------------------------------------------
class Text;
typedef std::shared_ptr<Text>       TextPtr;
typedef std::shared_ptr<const Text> ConstTextPtr;
typedef Property<TextPtr>           TextProperty;

// -----------------------------------------------------------------------------
class Text {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  String Content;
  String Font;
  Int    Size;

  // ----------------------------------------------------- contruction interface
  Text();

  static TextPtr create(std::string const& text = "",
                        std::string const& font = "sans",
                        int size = 18) {

    auto result = std::make_shared<Text>();
    result->Content = text;
    result->Font = font;
    result->Size = size;
    return result;
  }

  friend class FontRenderer;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  LayoutPtr const& get_layout();
  LayoutPtr layout_;

  bool font_update_;
  bool text_update_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_FONT_HPP
