////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/FullscreenSpriteComponent.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

FullscreenSpriteComponent::FullscreenSpriteComponent()
  : Depth(0.f)
  , Material(nullptr)
  , CustomMaterial(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void FullscreenSpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth       = Depth();
  s.Material    = Material() ? Material() : CustomMaterial();
  scene->renderers().fullscreen_sprite_renderer.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void FullscreenSpriteComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_object("Material", Material);
  visitor.add_object("CustomMaterial", CustomMaterial);
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
