////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_ANIMATED_GUI_SPRITE_COMPONENT_HPP
#define SWIFT2D_ANIMATED_GUI_SPRITE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/gui/GuiSpriteComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class AnimatedGuiSpriteComponent;
typedef std::shared_ptr<AnimatedGuiSpriteComponent>       AnimatedGuiSpriteComponentPtr;
typedef std::shared_ptr<const AnimatedGuiSpriteComponent> ConstAnimatedGuiSpriteComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL AnimatedGuiSpriteComponent : public GuiSpriteComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public GuiSpriteComponent::Serialized {
    float Time;
    bool  UseRenderThreadTime;
  };

  class Renderer : public ResourceRenderer<AnimatedGuiSpriteComponent> {
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  AnimatedFloat Time;
  Bool UseRenderThreadTime;

  // ---------------------------------------------------- construction interface
  AnimatedGuiSpriteComponent();

  static AnimatedGuiSpriteComponentPtr create() {
    return std::make_shared<AnimatedGuiSpriteComponent>();
  }

  // creates a copy from this
  AnimatedGuiSpriteComponentPtr create_copy() const {
    return std::make_shared<AnimatedGuiSpriteComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "AnimatedGuiSpriteComponent"; }

  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_ANIMATED_GUI_SPRITE_COMPONENT_HPP
