////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleSystem.hpp>

#include <swift2d/physics/Physics.hpp>
#include <swift2d/particles/ParticleUpdateShader.hpp>
#include <swift2d/textures/NoiseTexture.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

struct Particle {
  math::vec2 pos;
  math::vec2 vel;
  math::vec2 life;
};

////////////////////////////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem(int max_count)
  : particles_to_spawn_()
  , transform_feedbacks_()
  , particle_buffers_()
  , ping_(true)
  , total_time_(0.0)
  , update_max_count_(max_count) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::set_max_count(int max_count) {
  update_max_count_ = max_count;
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::upload_to(RenderContext const& ctx) {

  // allocate GPU resources
  for (int i(0); i<2; ++i) {
    transform_feedbacks_.push_back(ogl::TransformFeedback());
    particle_buffers_.push_back(ogl::Buffer());
    particle_vaos_.push_back(ogl::VertexArray());

    particle_vaos_[i].Bind();
    particle_buffers_[i].Bind(ose::Array());

    auto t(ogl::DataType::Float);
    auto s(sizeof(Particle));

    ogl::VertexArrayAttrib(0).Pointer(2, t, false, s, (void const*) 0);
    ogl::VertexArrayAttrib(1).Pointer(2, t, false, s, (void const*) 8);
    ogl::VertexArrayAttrib(2).Pointer(2, t, false, s, (void const*)16);
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::update_particles(
  std::unordered_set<ParticleEmitterComponentPtr> const& emitters,
  float mass, float linear_damping, float angular_damping, RenderContext const& ctx) {

  bool first_draw(false);

  // upload to GPU if neccessary
  if (particle_buffers_.empty()) {
    upload_to(ctx);
    timer_.start();
    first_draw = true;
  }

  // update buffer size if necessary
  if (update_max_count_ > 0) {
    for (int i(0); i<2; ++i) {
      particle_buffers_[i].Bind(ose::Array());

      std::vector<Particle> data(update_max_count_);
      data.front().pos  = math::vec2(0.f, 0.f);
      data.front().vel  = math::vec2(0.f, 0.f);
      data.front().life = math::vec2(0.f, 0.f);
      ogl::Buffer::Data(ose::Array(), data, ose::DynamicDraw());
    }

    update_max_count_ = 0;
  }

  double frame_time(timer_.reset());
  total_time_ += frame_time;

  // swap ping pong buffers
  ping_ = !ping_;

  ctx.gl.Enable(ogl::Capability::RasterizerDiscard);

  particle_vaos_      [current_vb()].Bind();
  transform_feedbacks_[current_tf()].Bind();
  particle_buffers_   [current_tf()].BindBase(ose::TransformFeedback(), 0);

  auto shader(ParticleUpdateShader::instance());
  shader->use(ctx);

  ogl::VertexArrayAttrib(0).Enable();
  ogl::VertexArrayAttrib(1).Enable();
  ogl::VertexArrayAttrib(2).Enable();

  transform_feedbacks_[current_tf()].BeginPoints();
  {

    // spawn new particles -----------------------------------------------------
    for (auto const& emitter: emitters) {

      // calculate spawn count
      particles_to_spawn_[emitter] += emitter->Density() * frame_time;
      int spawn_count(particles_to_spawn_[emitter]);
      particles_to_spawn_[emitter] -= spawn_count;

      if (spawn_count > 0) {
        math::vec2 life(emitter->Life(), emitter->LifeVariance());
        math::vec2 time(frame_time * 1000.0, total_time_ * 1000.0);
        math::vec2 direction(emitter->Direction(), emitter->DirectionVariance());
        math::vec2 velocity(emitter->Velocity(), emitter->VelocityVariance());
        NoiseTexture::instance()->bind(ctx, 0);
        shader->set_uniform("noise_tex",    0);
        shader->set_uniform("spawn_count",  spawn_count);
        shader->set_uniform("transform",    emitter->WorldTransform());
        shader->set_uniform("life",         life);
        shader->set_uniform("time",         time);
        shader->set_uniform("direction",    direction);
        shader->set_uniform("velocity",     velocity);
        ctx.gl.DrawArrays(ogl::PrimitiveType::Points, 0, 1);
      }
    }

    // update existing particles -----------------------------------------------
    if (!first_draw) {

      Physics::instance()->bind_gravity_map(ctx, 1);
      math::vec3 dynamics(mass, linear_damping, angular_damping);

      shader->set_uniform("gravity_map",  1);
      shader->set_uniform("spawn_count", -1);
      shader->set_uniform("projection",   ctx.projection_matrix);
      shader->set_uniform("dynamics",     dynamics);

      ctx.gl.DrawTransformFeedback(
        ogl::PrimitiveType::Points, transform_feedbacks_[current_vb()]
      );
    }

  }
  transform_feedbacks_[current_tf()].End();

  ogl::VertexArrayAttrib(0).Disable();
  ogl::VertexArrayAttrib(1).Disable();
  ogl::VertexArrayAttrib(2).Disable();

  ctx.gl.Disable(ogl::Capability::RasterizerDiscard);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::draw_particles(RenderContext const& ctx) {
  particle_vaos_[current_tf()].Bind();

  ogl::VertexArrayAttrib(0).Enable();
  ogl::VertexArrayAttrib(1).Enable();
  ogl::VertexArrayAttrib(2).Enable();

  ctx.gl.DrawTransformFeedback(
    ogl::PrimitiveType::Points, transform_feedbacks_[current_tf()]
  );

  ogl::VertexArrayAttrib(0).Disable();
  ogl::VertexArrayAttrib(1).Disable();
  ogl::VertexArrayAttrib(2).Disable();
}

////////////////////////////////////////////////////////////////////////////////

}
