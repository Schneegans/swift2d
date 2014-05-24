////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_FONT_RENDERER_HPP
#define SWIFT2D_FONT_RENDERER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/math.hpp>

#include <memory>
#include <unordered_map>

// forward declares ------------------------------------------------------------
namespace oglplus { namespace text {
  class PangoCairoRendering;
  class PangoCairoDefaultRenderer;
  class PangoCairoFont;
  class PangoCairoLayout;
}}

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
typedef std::shared_ptr<oglplus::text::PangoCairoFont>   FontPtr;
typedef std::shared_ptr<oglplus::text::PangoCairoLayout> LayoutPtr;

class Text;
typedef std::shared_ptr<Text> TextPtr;

// -----------------------------------------------------------------------------
class FontRenderer : public Singleton<FontRenderer> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void render(RenderContext const& ctx,
              TextPtr const& text,
              math::mat3 const& projection,
              math::mat3 const& transform);

  friend class Singleton<FontRenderer>;
  friend class Text;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  FontRenderer();
  ~FontRenderer();

  LayoutPtr make_layout(std::string const& font, int size, int max_width);
  FontPtr  get_font(std::string const& font, int size);

  oglplus::text::PangoCairoRendering       *rendering_;
  oglplus::text::PangoCairoDefaultRenderer *renderer_;

  std::unordered_map<std::string, FontPtr> fonts_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_FONT_RENDERER_HPP
