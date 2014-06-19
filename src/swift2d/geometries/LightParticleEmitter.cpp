////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/LightParticleEmitter.hpp>

#include <swift2d/materials/LightParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LightParticleEmitter::LightParticleEmitter()
  : ParticleEmitter() {}

////////////////////////////////////////////////////////////////////////////////

void LightParticleEmitter::draw(RenderContext const& ctx, math::mat3 const& transform, int count) const {

  auto shader(LightParticleShader::instance());

  LightParticleShader::instance()->use(ctx);
  Texture()->bind(ctx, 0);

  // set uniforms
  if (WorldSpacePosition()) {
    auto scale(math::make_scale(math::get_scale(transform)));
    shader->set_uniform("transform",    scale);
  } else {
    shader->set_uniform("transform",    transform);
  }

  shader->set_uniform("light_tex",      0);
  shader->set_uniform("projection",     ctx.projection_matrix);

  shader->set_uniform("start_scale",    StartScale());
  shader->set_uniform("end_scale",      EndScale());

  shader->set_uniform("start_color",    StartColor().vec3());
  shader->set_uniform("end_color",      EndColor().vec3());

  shader->set_uniform("start_opacity",  StartOpacity());
  shader->set_uniform("end_opacity",    EndOpacity());

  shader->set_uniform("screen_size",    ctx.size);
  shader->set_uniform("g_buffer_normal", 2);

  // draw
  ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, count);
}

////////////////////////////////////////////////////////////////////////////////

void LightParticleEmitter::spawn(math::mat3 transform, math::vec2& position,
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
  rotation = 0.f;
  rotation_speed = 0.f;

  // generate life time
  var = LifeVariance();
  max_age = std::max(0.f, Life() + math::random::get(-var, var));
}

////////////////////////////////////////////////////////////////////////////////

}
