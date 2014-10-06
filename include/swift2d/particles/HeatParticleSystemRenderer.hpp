////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_HEAT_PARTICLE_SYSTEM_RENDERER_HPP
#define SWIFT2D_HEAT_PARTICLE_SYSTEM_RENDERER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/particles/HeatParticleSystemComponent.hpp>
#include <swift2d/particles/HeatParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL HeatParticleSystemRenderer : public ResourceRenderer<HeatParticleSystemComponent> {

  void draw_impl(RenderContext const& ctx, int start, int end) {
    for (int i(start); i<end; ++i) {
      auto& o(objects[i]);

      SWIFT_PUSH_GL_RANGE("Draw HeatParticleSystem");

      if (o.System->update_particles(o, ctx) > 0) {

        o.Texture->bind(ctx, 0);

        auto& shader(HeatParticleShader::get());
        shader.use(ctx);
        shader.projection. Set(ctx.projection_matrix);
        shader.diffuse.    Set(0);
        shader.scale.      Set(math::vec2(o.StartScale, o.EndScale));
        shader.opacity.    Set(math::vec2(o.StartOpacity, o.EndOpacity));

        o.System->draw_particles(ctx);
      }

      SWIFT_POP_GL_RANGE();
    }
  }
};

}

#endif  // SWIFT2D_HEAT_PARTICLE_SYSTEM_RENDERER_HPP
