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
#include <swift2d/graphics/Pipeline.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

AnimatedSpriteComponent::AnimatedSpriteComponent()
  : SpriteComponent()
  , Time(0.f, 1.0f, 10.0f)
  , UseRenderThreadTime(false) {}

////////////////////////////////////////////////////////////////////////////////

void AnimatedSpriteComponent::update(double time) {
  SpriteComponent::update(time);
  Time.update(time);
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedSpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Transform   = WorldTransform();
  s.Depth       = WorldDepth();
  s.Time        = Time();
  s.UseRenderThreadTime = UseRenderThreadTime();
  s.Material    = CustomMaterial() ? CustomMaterial() : Material();
  scene->renderers().animated_sprites.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedSpriteComponent::accept(SavableObjectVisitor& visitor) {
  SpriteComponent::accept(visitor);
  DepthComponent::accept(visitor);
  visitor.add_object("Time", Time);
  visitor.add_member("UseRenderThreadTime", UseRenderThreadTime);
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

    while (start < end && objects[start].Material == mat) {
      transforms.push_back(objects[start].Transform);
      if (objects[start].UseRenderThreadTime) {
        times.push_back(ctx.pipeline->get_total_time() - (int)ctx.pipeline->get_total_time());
      } else {
        times.push_back(objects[start].Time);
      }
      ++start;
    }

    mat->draw_quads(ctx, transforms, object.Depth, times);

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
