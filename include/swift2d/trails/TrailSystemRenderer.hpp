////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TRAIL_SYSTEM_RENDERER_HPP
#define SWIFT2D_TRAIL_SYSTEM_RENDERER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/trails/TrailSystemComponent.hpp>
#include <swift2d/trails/TrailSystem.hpp>
#include <swift2d/trails/TexturedTrailShader.hpp>
#include <swift2d/trails/ColoredTrailShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL TrailSystemRenderer : public ResourceRenderer<TrailSystemComponent> {

  void draw(RenderContext const& ctx, int start, int end) {

    for (int i(start); i<end; ++i) {
      auto& o(objects[i]);

      SWIFT_PUSH_GL_RANGE("Draw TrailSystem");
      o.System->update_trails(o.Emitters, o, ctx);

      if (o.BlendAdd) {
        ctx.gl.BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::One);
      }

      if (o.Texture) {
        auto& shader(TexturedTrailShader::get());
        shader.use(ctx);
        o.Texture->bind(ctx, 0);
        shader.texture.Set(0);
        shader.texture_repeat.        Set(o.TextureRepeat);
        shader.projection.            Set(ctx.projection_matrix);
        shader.start_width.           Set(o.StartWidth);
        shader.end_width.             Set(o.EndWidth);
        shader.start_color.           Set(o.StartColor);
        shader.end_color.             Set(o.EndColor);
        shader.glow.                  Set(math::vec2(o.StartGlow, o.EndGlow));
        shader.use_global_texcoords.  Set(o.UseGlobalTexCoords ? 1 : 0);
        shader.total_time.            Set(o.System->get_total_time() * 1000.0);
      } else {
        auto& shader(ColoredTrailShader::get());
        shader.use(ctx);
        shader.projection.            Set(ctx.projection_matrix);
        shader.start_width.           Set(o.StartWidth);
        shader.end_width.             Set(o.EndWidth);
        shader.start_color.           Set(o.StartColor);
        shader.end_color.             Set(o.EndColor);
        shader.glow.                  Set(math::vec2(o.StartGlow, o.EndGlow));
        shader.use_global_texcoords.  Set(o.UseGlobalTexCoords ? 1 : 0);
        shader.total_time.            Set(o.System->get_total_time() * 1000.0);
      }

      o.System->draw_trails(o.Emitters, o, ctx);

      if (o.BlendAdd) {
        ctx.gl.BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::OneMinusSrcAlpha);
      }

      SWIFT_POP_GL_RANGE();
    }
  }
};

}

#endif  // SWIFT2D_TRAIL_SYSTEM_RENDERER_HPP
