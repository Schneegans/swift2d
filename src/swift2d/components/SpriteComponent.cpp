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

  auto& mat(Material() ? Material() : CustomMaterial());

  if (FullScreen()) {
    mat->draw_fullscreen_quad(ctx);
  } else {
    mat->draw_quad(ctx, WorldTransform(), Depth());
  }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::draw_instanced(RenderContext const& ctx, std::vector<math::mat3> const& transforms) {

  auto& mat(Material() ? Material() : CustomMaterial());

  if (FullScreen()) {
    mat->draw_fullscreen_quad(ctx);
  } else {

    int index(0);

    while (index < transforms.size()) {
      int count(std::min(100, (int)transforms.size()-index));
      mat->draw_quads(ctx, std::vector<math::mat3>(transforms.begin() + index, transforms.begin() + index + count), Depth());

      index += count;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteComponent::serialize(SerializedScenePtr& scene) const {
  // scene->objects[Depth.get()].add_object(create_copy());
  scene->objects[Depth.get()].add_instanced_object(Material() ? Material().get() : CustomMaterial().get(), create_copy());
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
