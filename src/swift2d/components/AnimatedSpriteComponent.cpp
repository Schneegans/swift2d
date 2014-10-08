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

void AnimatedSpriteComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {

  std::sort(objects.begin() + start, objects.begin() + end,
    [](AnimatedSpriteComponent::Serialized const& a, AnimatedSpriteComponent::Serialized const& b){
      return a.Material.get() < b.Material.get();
    });

  while (start < end) {

    auto& object(objects[start]);
    auto& mat(object.Material);

    SWIFT_PUSH_GL_RANGE("Draw AnimatedSprite");

    std::vector<math::mat3> transforms;
    std::vector<float>      times;

    while (objects[start].Material == mat && start < end) {
      transforms.push_back(objects[start].Transform);
      times.push_back(objects[start].Time);
      ++start;
    }

    mat->draw_quads(ctx, transforms, object.Depth, times);

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
