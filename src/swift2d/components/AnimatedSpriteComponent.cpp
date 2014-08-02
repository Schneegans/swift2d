////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/AnimatedSpriteComponent.hpp>

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

void AnimatedSpriteComponent::draw(RenderContext const& ctx) {
  if (FullScreen()) {
    Material()->draw_fullscreen_quad(ctx, Time());
  } else {
    Material()->draw_quad(ctx, WorldTransform(), Depth(), Time());
  }
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedSpriteComponent::serialize(SerializedScenePtr& scene) const {
  scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedSpriteComponent::accept(SavableObjectVisitor& visitor) {
  SpriteComponent::accept(visitor);
  visitor.add_member("Time", Time);
}

////////////////////////////////////////////////////////////////////////////////

}
