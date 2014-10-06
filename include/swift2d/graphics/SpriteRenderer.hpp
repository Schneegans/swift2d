////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_RENDERER_HPP
#define SWIFT2D_SPRITE_RENDERER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/components/SpriteComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL SpriteRenderer : public ResourceRenderer<SpriteComponent> {

  void draw_impl(RenderContext const& ctx, int start, int end) {
    std::sort(objects.begin() + start, objects.begin() + end,
      [](SpriteComponent::Serialized const& a, SpriteComponent::Serialized const& b){
        return a.Material.get() < b.Material.get();
      });

    while (start < end) {

      auto& object(objects[start]);
      auto& mat(object.Material);

      SWIFT_PUSH_GL_RANGE("Draw Sprite");

      if (object.FullScreen) {
        mat->draw_fullscreen_quad(ctx);
        ++start;

      } else {
        std::vector<math::mat3> transforms;

        while (objects[start].Material == mat && start < end) {
          transforms.push_back(objects[start].Transform);
          ++start;
        }

        mat->draw_quads(ctx, transforms, object.Depth);
      }
      SWIFT_POP_GL_RANGE();
    }
  }
};

}

#endif  // SWIFT2D_SPRITE_RENDERER_HPP
