////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_POINT_PARTICLE_SYSTEM_RENDERER_HPP
#define SWIFT2D_POINT_PARTICLE_SYSTEM_RENDERER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/particles/PointParticleSystemComponent.hpp>
#include <swift2d/particles/PointParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL PointParticleSystemRenderer : public ResourceRenderer<PointParticleSystemComponent> {

  void draw(RenderContext const& ctx, int start, int end) {
    for (int i(start); i<end; ++i) {
      auto& o(objects[i]);

      SWIFT_PUSH_GL_RANGE("Draw PointParticleSystem");

      if (o.System->update_particles(o, ctx) > 0) {
        ctx.gl.Rasterization::PointSize(o.Scale);

        if (o.BlendAdd) {
          ctx.gl.BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::One);
        }

        auto& shader(PointParticleShader::get());
        shader.use(ctx);
        shader.projection. Set(ctx.projection_matrix);
        shader.start_color.Set(o.StartColor);
        shader.end_color.  Set(o.EndColor);
        shader.glow.       Set(math::vec2(o.StartGlow, o.EndGlow));

        o.System->draw_particles(ctx);

        if (o.BlendAdd) {
          ctx.gl.BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::OneMinusSrcAlpha);
        }
      }

      SWIFT_POP_GL_RANGE();
    }
  }
};

}

#endif  // SWIFT2D_POINT_PARTICLE_SYSTEM_RENDERER_HPP
