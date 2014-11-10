////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/GuiSpriteComponent.hpp>

#include <swift2d/gui/GuiShader.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

GuiSpriteComponent::GuiSpriteComponent()
  : FullscreenGuiSpriteComponent()
  , Size(math::vec2i(10,10))
  , Anchor(math::vec2i(0,0))
  , Offset(math::vec2i(0,0)) {}

////////////////////////////////////////////////////////////////////////////////

void GuiSpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth    = Depth();
  s.Opacity  = Opacity();
  s.Size     = Size();
  s.Anchor   = Anchor();
  s.Offset   = Offset();
  s.Texture  = Texture();
  scene->renderers().gui_sprite_elements.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void GuiSpriteComponent::accept(SavableObjectVisitor& visitor) {
  FullscreenGuiSpriteComponent::accept(visitor);
  visitor.add_member("Size",             Size);
  visitor.add_member("Anchor",           Anchor);
  visitor.add_member("Offset",           Offset);
}

////////////////////////////////////////////////////////////////////////////////

}