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
  , Material(nullptr)
  , CustomMaterial(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::draw(RenderContext const& ctx) {
  SWIFT_PUSH_GL_RANGE("Draw Sprite");
  auto& mat(Material() ? Material() : CustomMaterial());

  if (FullScreen()) {
    mat->draw_fullscreen_quad(ctx);
  } else {
    mat->draw_quad(ctx, WorldTransform(), Depth());
  }
  SWIFT_POP_GL_RANGE();
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
  visitor.add_object("CustomMaterial", CustomMaterial);
}

////////////////////////////////////////////////////////////////////////////////

}
