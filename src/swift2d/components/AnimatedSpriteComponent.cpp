////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/AnimatedSpriteComponent.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

AnimatedSpriteComponent::AnimatedSpriteComponent()
  : SpriteComponent()
  , Time(0.f, 1.0f, 10.0f) {}

////////////////////////////////////////////////////////////////////////////////

void AnimatedSpriteComponent::update(double time) {
  SpriteComponent::update(time);

  Time.update(time);
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedSpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth       = Depth();
  s.Time        = Time();
  s.Transform   = WorldTransform();
  s.Material    = Material() ? Material() : CustomMaterial();
  scene->renderers().animated_sprite_renderer.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedSpriteComponent::accept(SavableObjectVisitor& visitor) {
  SpriteComponent::accept(visitor);
  visitor.add_member("Time", Time);
}

////////////////////////////////////////////////////////////////////////////////

}
