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
  math::vec2 rot;
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

    for (long i(0); i<4; ++i) {
      ogl::VertexArrayAttrib(i).Pointer(2, ogl::DataType::Float, false, sizeof(Particle), (void const*) (i*8));
      ogl::VertexArrayAttrib(i).Enable();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::update_particles(
  std::vector<SerializedEmitter> const& emitters,
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
      data.front().rot  = math::vec2(0.f, 0.f);
      ogl::Buffer::Data(ose::Array(), data, ose::DynamicDraw());
    }

    update_max_count_ = 0;
  }

  // get frame time
  double frame_time(timer_.reset());
  total_time_ += frame_time;

  // swap ping pong buffers
  ping_ = !ping_;

  ctx.gl.Enable(ogl::Capability::RasterizerDiscard);

  particle_vaos_      [current_vb()].Bind();
  transform_feedbacks_[current_tf()].Bind();
  particle_buffers_   [current_tf()].BindBase(ose::TransformFeedback(), 0);

  auto& shader(ParticleUpdateShader::get());
  shader.use(ctx);

  transform_feedbacks_[current_tf()].BeginPoints();
  {

    // spawn new particles -----------------------------------------------------
    for (auto const& emitter: emitters) {

      // calculate spawn count
      int spawn_count(0);

      if (emitter.Self) {
        particles_to_spawn_[emitter.Self] += emitter.Density * frame_time;
        spawn_count = particles_to_spawn_[emitter.Self];
        particles_to_spawn_[emitter.Self] -= spawn_count;
      } else {
        spawn_count = emitter.Density;
      }

      if (spawn_count > 0) {
        math::vec2 time       (frame_time * 1000.0, total_time_ * 1000.0);
        math::vec2 life       (emitter.Life,            emitter.LifeVariance);
        math::vec2 direction  (emitter.Direction,       emitter.DirectionVariance);
        math::vec2 velocity   (emitter.Velocity,        emitter.VelocityVariance);
        math::vec2 rotation   (emitter.AngularVelocity, emitter.AngularVelocityVariance);
        NoiseTexture::get().bind(ctx, 0);

        shader.noise_tex.        Set(0);
        shader.spawn_count.      Set(spawn_count);
        shader.transform.        Set(emitter.WorldTransform);
        shader.position_variance.Set(emitter.PositionVariance);

        shader.time.             Set(time);
        shader.life.             Set(life);
        shader.direction.        Set(direction);
        shader.velocity.         Set(velocity);
        shader.rotation.         Set(rotation);

        ctx.gl.DrawArrays(ogl::PrimitiveType::Points, 0, 1);
      }
    }

    // update existing particles -----------------------------------------------
    if (!first_draw) {
      Physics::get().bind_gravity_map(ctx, 0);
      math::vec3 dynamics(mass, linear_damping, angular_damping);

      shader.gravity_map.        Set(0);
      shader.spawn_count.        Set(-1);
      shader.projection.         Set(ctx.projection_matrix);
      shader.dynamics.           Set(dynamics);

      ctx.gl.DrawTransformFeedback(
        ogl::PrimitiveType::Points, transform_feedbacks_[current_vb()]
      );
    }

  }
  transform_feedbacks_[current_tf()].End();

  ctx.gl.Disable(ogl::Capability::RasterizerDiscard);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::draw_particles(RenderContext const& ctx) {

  particle_vaos_[current_tf()].Bind();

  ctx.gl.DrawTransformFeedback(
    ogl::PrimitiveType::Points, transform_feedbacks_[current_tf()]
  );
}

////////////////////////////////////////////////////////////////////////////////

}
