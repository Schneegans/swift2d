////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/SpriteComponent.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteComponent::SpriteComponent()
  : Material(nullptr)
  , CustomMaterial(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::update(double time) {
  TransformableComponent::update(time);
  DepthComponent::update(time, get_user());
}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth       = WorldDepth();
  s.Transform   = WorldTransform();
  s.Material    = Material() ? Material() : CustomMaterial();
  scene->renderers().sprites.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  DepthComponent::accept(visitor);
  visitor.add_object_property("Material", Material);
  visitor.add_object_property("CustomMaterial", CustomMaterial);
}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  std::sort(objects.begin() + start, objects.begin() + end,
    [](SpriteComponent::Serialized const& a, SpriteComponent::Serialized const& b){
      return a.Material.get() < b.Material.get();
    });

  while (start < end) {

    auto& object(objects[start]);
    auto& mat(object.Material);

    SWIFT_PUSH_GL_RANGE("Draw Sprite");

    std::vector<math::mat3> transforms;

    while (start < end && objects[start].Material == mat) {
      transforms.push_back(objects[start].Transform);
      ++start;
    }

    mat->draw_quads(ctx, transforms, object.Depth);

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
