////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GUI_SPRITE_COMPONENT_HPP
#define SWIFT2D_GUI_SPRITE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/gui/FullscreenGuiSpriteComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class GuiSpriteComponent;
typedef std::shared_ptr<GuiSpriteComponent>       GuiSpriteComponentPtr;
typedef std::shared_ptr<const GuiSpriteComponent> ConstGuiSpriteComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL GuiSpriteComponent : public FullscreenGuiSpriteComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Vec2i  Size;
  Vec2   Anchor;
  Vec2   Offset;


  // ---------------------------------------------------- construction interface
  GuiSpriteComponent();

  static GuiSpriteComponentPtr create() {
    return std::make_shared<GuiSpriteComponent>();
  }

  // creates a copy from this
  GuiSpriteComponentPtr create_copy() const {
    return std::make_shared<GuiSpriteComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
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
