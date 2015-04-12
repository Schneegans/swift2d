////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/FullscreenSpriteComponent.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

FullscreenSpriteComponent::FullscreenSpriteComponent()
  : Material(nullptr)
  , CustomMaterial(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void FullscreenSpriteComponent::update(double time) {
  Component::update(time);
  DepthComponent::update(time, get_user());
}

////////////////////////////////////////////////////////////////////////////////

void FullscreenSpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth       = WorldDepth();
  s.Material    = Material() ? Material() : CustomMaterial();
  scene->renderers().fullscreen_sprites.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void FullscreenSpriteComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  DepthComponent::accept(visitor);
  visitor.add_object_property("Material", Material);
  visitor.add_object_property("CustomMaterial", CustomMaterial);
}

////////////////////////////////////////////////////////////////////////////////

void FullscreenSpriteComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  std::sort(objects.begin() + start, objects.begin() + end,
    [](FullscreenSpriteComponent::Serialized const& a, FullscreenSpriteComponent::Serialized const& b){
      return a.Material.get() < b.Material.get();
    });

  for (int i(start); i<end; ++i) {
    SWIFT_PUSH_GL_RANGE("Draw FullscreenSprite");
    objects[i].Material->draw_fullscreen_quad(ctx);
    ++start;
    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
