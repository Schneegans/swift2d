////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/TextureParticleEmitter.hpp>

#include <swift2d/materials/TextureParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TextureParticleEmitter::TextureParticleEmitter()
  : ParticleEmitter()
  , BlendAdditive         (false)
  , RotationSpeed         (0.f)
  , RotationSpeedVariance (0.f)
  , Rotation              (0.f)
  , RotationVariance      (0.f)
  , StartGlow             (0.f)
  , EndGlow               (0.f) {}

////////////////////////////////////////////////////////////////////////////////

void TextureParticleEmitter::draw(RenderContext const& ctx, math::mat3 const& transform, int count) const {

  auto shader(TextureParticleShader::instance());

  TextureParticleShader::instance()->use(ctx);
  Texture()->bind(ctx, 0);

  // set uniforms
  if (WorldSpacePosition()) {
    auto scale(math::make_scale(math::get_scale(transform)));
    shader->transform.Set(scale);
  } else {
    shader->transform.Set(transform);
  }

  shader->diffuse.Set(0);
  shader->projection.Set(ctx.projection_matrix);

  shader->start_scale.Set(StartScale());
  shader->end_scale.Set(EndScale());

  shader->start_color.Set(StartColor().vec3());
  shader->end_color.Set(EndColor().vec3());

  shader->start_opacity.Set(StartOpacity());
  shader->end_opacity.Set(EndOpacity());

  shader->start_glow.Set(StartGlow());
  shader->end_glow.Set(EndGlow());

  shader->enable_rotation.Set((int)(
    RotationSpeed() + RotationSpeedVariance() +
    Rotation() + RotationVariance()
  ));

  // draw
  if (BlendAdditive()) {
    ctx.gl.BlendFunc(oglplus::BlendFn::SrcAlpha, oglplus::BlendFn::One);
    ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, count);
    ctx.gl.BlendFunc(oglplus::BlendFn::SrcAlpha, oglplus::BlendFn::OneMinusSrcAlpha);
  } else {
    ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, count);
  }
}

////////////////////////////////////////////////////////////////////////////////

void TextureParticleEmitter::spawn(math::mat3 transform, math::vec2& position,
                            float& max_age, math::vec2& direction,
                            float& rotation, float& rotation_speed) const {

  // scale of parent
  math::vec2 scale(math::get_scale(transform));

  // generate position
  if (WorldSpacePosition()) {
    math::vec2 pos(Position());
    position     = (transform * math::vec3(pos.x(), pos.y(), 1)).xy();
    position[0] /= scale.x();
    position[1] /= scale.y();
  } else {
    position = Position();
  }

  // generate direction
  if (WorldSpaceDirection()) {
    direction = Direction();
  } else {
    math::vec2 dir(Direction());
    direction     = (transform * math::vec3(dir.x(), dir.y(), 0)).xy();
    direction[0] /= scale.x();
    direction[1] /= scale.y();
  }

  // influence of direction variance
  float var(DirectionVariance());
  auto dir_rot = math::make_rotate(math::random::get(-var, var));
  direction = (dir_rot * math::vec3(direction[0], direction[1], 0.0f)).xy();

  // generate rotation
  var = RotationVariance();
  rotation = Rotation() + math::random::get(-var, var);

  var = RotationSpeedVariance();
  rotation_speed = RotationSpeed() + math::random::get(-var, var);

  // generate life time
  var = LifeVariance();
  max_age = std::max(0.f, Life() + math::random::get(-var, var));
}

////////////////////////////////////////////////////////////////////////////////

}
