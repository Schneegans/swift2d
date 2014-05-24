////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TEXT_COMPONENT_HPP
#define SWIFT2D_TEXT_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/materials/Material.hpp>
#include <swift2d/fonts/Text.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class TextComponent;
typedef std::shared_ptr<TextComponent>       TextComponentPtr;
typedef std::shared_ptr<const TextComponent> ConstTextComponentPtr;

// -----------------------------------------------------------------------------
class TextComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  TextPtr Text;
  Bool    InScreenSpace;
  Float   Depth;

  // ----------------------------------------------------- contruction interface
  TextComponent()
    : Depth(0.f)
    , InScreenSpace(true) {}

  // Creates a new component and returns a shared pointer.
  static TextComponentPtr create() {
    return std::make_shared<TextComponent>();
  }

  static TextComponentPtr create(TextPtr const& text) {
    auto component = std::make_shared<TextComponent>();
    component->Text = text;
    return component;
  }

  // creates a copy from this
  TextComponentPtr create_copy() const {
    return std::make_shared<TextComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  void draw(RenderContext const& ctx) {

    if (InScreenSpace()) {
      int font_size(Text->Size());
      math::vec2 scale(2.0*font_size/ctx.size.x(), 2.0*font_size/ctx.size.y());
      auto world_pos(math::get_translate(WorldTransform()));
      auto screen_pos(ctx.projection_matrix * math::vec3(world_pos.x()/scale.x(), world_pos.y()/scale.y(), 1));
      math::vec2 pixel_position(std::round(screen_pos.x()*font_size)/font_size, std::round(screen_pos.y()*font_size)/font_size);

      FontRenderer::instance()->render(ctx, Text, math::make_scale(scale), math::make_translate(pixel_position.x(), pixel_position.y()));
    } else {
      FontRenderer::instance()->render(ctx, Text, ctx.projection_matrix, WorldTransform());
    }
  }

  void serialize(SerializedScenePtr& scene) const {
    scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
  }

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_TEXT_COMPONENT_HPP
