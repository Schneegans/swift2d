////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/SpriteComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteComponent::SpriteComponent()
  : Depth(0.f)
  , FullScreen(false)
  , Material(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::draw(RenderContext const& ctx) {
  if (FullScreen()) {
    Material()->draw_fullscreen_quad(ctx);
  } else {
    Material()->draw_quad(ctx, WorldTransform(), Depth());
  }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::serialize(SerializedScenePtr& scene) const {
  scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::accept(SavableObjectVisitor& visitor) {
  DrawableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_member("FullScreen", FullScreen);
  visitor.add_object("Material", Material);
}

////////////////////////////////////////////////////////////////////////////////

}
