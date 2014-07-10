////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/HeatParticleEmitter.hpp>

#include <swift2d/materials/HeatParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void HeatParticleEmitter::draw(RenderContext const& ctx, math::mat3 const& transform, int count) const {

  auto shader(HeatParticleShader::instance());

  HeatParticleShader::instance()->use(ctx);
  Texture()->bind(ctx, 0);

  // set uniforms
  if (WorldSpacePosition()) {
    auto scale(math::make_scale(math::get_scale(transform)));
    shader->transform.Set(scale);
  } else {
    shader->transform.Set(transform);
  }

  shader->heat_tex.Set(0);
  shader->projection.Set(ctx.projection_matrix);

  shader->start_scale.Set(StartScale());
  shader->end_scale.Set(EndScale());

  shader->start_opacity.Set(StartOpacity());
  shader->end_opacity.Set(EndOpacity());

  // draw
  ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, count);
}

////////////////////////////////////////////////////////////////////////////////

}
