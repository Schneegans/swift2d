////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GUI_SPRITE_COMPONENT_HPP
#define SWIFT2D_GUI_SPRITE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/textures/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class GuiSpriteComponent;
typedef std::shared_ptr<GuiSpriteComponent>       GuiSpriteComponentPtr;
typedef std::shared_ptr<const GuiSpriteComponent> ConstGuiSpriteComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL GuiSpriteComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public SerializedComponent {
    math::vec2i     Size;
    math::vec2      Anchor;
    math::vec2      Offset;
    TexturePtr      Texture;
  };

  class Renderer : public ResourceRenderer<GuiSpriteComponent> {
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  Float  Depth;
  Vec2i  Size;
  Vec2   Anchor;
  Vec2   Offset;
  TextureProperty Texture;

  // ---------------------------------------------------- construction interface
  GuiSpriteComponent();

  static GuiSpriteComponentPtr create() {
    return std::make_shared<GuiSpriteComponent>();
  }

  // creates a copy from this
  GuiSpriteComponentPtr create_copy() const {
    return std::make_shared<GuiSpriteComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "GuiSpriteComponent"; }

  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_GUI_SPRITE_COMPONENT_HPP
