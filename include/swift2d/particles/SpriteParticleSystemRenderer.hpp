////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_PARTICLE_SYSTEM_RENDERER_HPP
#define SWIFT2D_SPRITE_PARTICLE_SYSTEM_RENDERER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/particles/SpriteParticleSystemComponent.hpp>
#include <swift2d/particles/SpriteParticleShader.hpp>
#include <swift2d/particles/ParticleSystem.hpp>
#include <swift2d/graphics/Pipeline.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL SpriteParticleSystemRenderer : public ResourceRenderer<SpriteParticleSystemComponent> {

  void predraw(RenderContext const& ctx) {
    for (auto& object : objects) {
      object.System->update_particles(object, ctx);
    }
  }

  void draw(RenderContext const& ctx, int start, int end) {
    for (int i(start); i<end; ++i) {
      auto& o(objects[i]);

      SWIFT_PUSH_GL_RANGE("Draw SpriteParticleSystemRenderer");

      if (o.System->get_particle_count() > 0) {

        if (o.SubSamplingLevel > 1) {
          ctx.pipeline->get_sub_sampler(o.SubSamplingLevel)->bind(ctx, o.BlendAdd);
        } else if (o.BlendAdd) {
          ctx.gl.BlendFunc(ose::SrcAlpha(), ose::One());
        }

        o.Texture->bind(ctx, 0);

        auto& shader(SpriteParticleShader::get());
        shader.use(ctx);
        shader.projection. Set(ctx.projection_matrix);
        shader.diffuse.    Set(0);
        shader.scale.      Set(math::vec4(o.StartScale.x(), o.StartScale.y(), o.EndScale.x(), o.EndScale.y()));
        shader.glow.       Set(math::vec2(o.StartGlow, o.EndGlow));
        shader.start_color.Set(o.StartColor);
        shader.end_color.  Set(o.EndColor);

        o.System->draw_particles(ctx);

        if (o.SubSamplingLevel > 1) {
          ctx.pipeline->get_sub_sampler(o.SubSamplingLevel)->draw(ctx, o.BlendAdd);
        } else if (o.BlendAdd) {
          ctx.gl.BlendFunc(ose::SrcAlpha(), ose::OneMinusSrcAlpha());
        }
      }

      SWIFT_POP_GL_RANGE();
    }
  }
};

}

#endif  // SWIFT2D_SPRITE_PARTICLE_SYSTEM_RENDERER_HPP
