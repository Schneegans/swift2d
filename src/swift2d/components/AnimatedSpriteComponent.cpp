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

void AnimatedSpriteComponent::draw_instanced(RenderContext const& ctx, std::vector<math::mat3> const& transforms) {
  if (FullScreen()) {
    Material()->draw_fullscreen_quad(ctx, Time());
  } else {
    
    int index(0);

    while (index < transforms.size()) {
      int count(std::min(100, (int)transforms.size()-index));
      Material()->draw_quads(ctx, std::vector<math::mat3>(transforms.begin() + index, transforms.begin() + index + count), Depth(), Time());

      index += count;
    }

  }
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedSpriteComponent::serialize(SerializedScenePtr& scene) const {
  // scene->objects[Depth.get()].add_object(create_copy());
  scene->objects[Depth.get()].add_instanced_object(Material().get(), create_copy());
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedSpriteComponent::accept(SavableObjectVisitor& visitor) {
  SpriteComponent::accept(visitor);
  visitor.add_member("Time", Time);
}

////////////////////////////////////////////////////////////////////////////////

}
