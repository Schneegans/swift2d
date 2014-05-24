////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/fonts/FontRenderer.hpp>
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/fonts/Text.hpp>
#include <swift2d/graphics/Window.hpp>

#include <oglplus/text/pango_cairo.hpp>
#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

FontRenderer::FontRenderer()
  : rendering_(new oglplus::text::PangoCairoRendering(0))
  , renderer_(new oglplus::text::PangoCairoRendering::Renderer(rendering_->GetRenderer(
      oglplus::FragmentShader(
        oglplus::ObjectDesc("Pixel color"),
        oglplus::StrLit(
          "#version 330\n"
          "vec4 PixelColor("
          " vec4 TexelColor,"
          " vec3 GlyphPosition,"
          " float GlyphXOffset,"
          " vec2 GlyphExtent,"
          " vec2 GlyphCoord,"
          " float LayoutWidth"
          ")"
          "{"
          " return vec4(1, 1, 1, TexelColor.r);"
          "}"
        )
      )))) {

  renderer_->Use();
  renderer_->SetAlignment(oglplus::text::Alignment::Left);
  renderer_->SetDirection(oglplus::text::Direction::LeftToRight);
}

////////////////////////////////////////////////////////////////////////////////

FontRenderer::~FontRenderer() {
  if (rendering_) delete rendering_;
  if (renderer_)  delete renderer_;
}

////////////////////////////////////////////////////////////////////////////////

void FontRenderer::render(RenderContext const& ctx, TextPtr const& text, math::mat3 const& projection, math::mat3 const& transform) {

  renderer_->Use();
  renderer_->SetProjection(math::to_3D(projection));
  renderer_->SetLayoutTransform(math::to_3D(transform));
  renderer_->Render(*text->get_layout());
}

////////////////////////////////////////////////////////////////////////////////

LayoutPtr FontRenderer::make_layout(std::string const& font, int size, int max_width) {
  return std::make_shared<oglplus::text::PangoCairoRendering::Layout>(rendering_->MakeLayout(*get_font(font, size), max_width));
}

////////////////////////////////////////////////////////////////////////////////

FontPtr FontRenderer::get_font(std::string const& name, int size) {
  std::stringstream sstr;
  sstr << name << " " << size;
  std::string font_name(sstr.str());

  auto font(fonts_.find(font_name));
  if (font == fonts_.end()) {
    auto new_font = std::make_shared<oglplus::text::PangoCairoRendering::Font>(font_name.c_str());
    fonts_[font_name] = new_font;
    return new_font;
  }

  return font->second;
}

////////////////////////////////////////////////////////////////////////////////

}

