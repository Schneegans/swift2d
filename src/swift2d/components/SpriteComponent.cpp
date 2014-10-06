////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/SpriteComponent.hpp>
#include <swift2d/graphics/SpriteRenderer.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteComponent::SpriteComponent()
  : Depth(0.f)
  , FullScreen(false)
  , Material(nullptr)
  , CustomMaterial(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth       = Depth();
  s.Transform   = WorldTransform();
  s.FullScreen  = FullScreen();
  s.Material    = Material() ? Material() : CustomMaterial();
  scene->renderers().sprite_renderer.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_member("FullScreen", FullScreen);
  visitor.add_object("Material", Material);
  visitor.add_object("CustomMaterial", CustomMaterial);
}

////////////////////////////////////////////////////////////////////////////////

}
