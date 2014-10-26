////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleSystem.hpp>

#include <swift2d/particles/ParticleSystemComponent.hpp>
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
  , update_max_count_(max_count)
  , count_(0) {}

////////////////////////////////////////////////////////////////////////////////

ParticleSystem::~ParticleSystem() {
  delete query_;
}

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

    for (long j(0); j<4; ++j) {
      ogl::VertexArrayAttrib(j).Enable();
      ogl::VertexArrayAttrib(j).Pointer(2, ogl::DataType::Float, false, sizeof(Particle), (void const*) (j*2*sizeof(float)));
    }

    ogl::NoVertexArray().Bind();
  }

  query_ = new ogl::Query();
}

////////////////////////////////////////////////////////////////////////////////

int ParticleSystem::update_particles(ParticleSystemComponent::Serialized const& system,
  RenderContext const& ctx) {

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
      ogl::Buffer::Data(ose::Array(), data, ose::StreamCopy());
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

  count_ = 0;
  if (!first_draw) {
    query_->Result(count_);
  }

  std::vector<math::vec3> spawn_positions;
  for (auto const& emitter: system.Emitters) {

    // calculate spawn count
    int spawn_count(0);

    if (emitter.Self) {
      particles_to_spawn_[emitter.Self] += emitter.Density * frame_time;
      spawn_count = particles_to_spawn_[emitter.Self];
      particles_to_spawn_[emitter.Self] -= spawn_count;
    } else {
      spawn_count = emitter.Density;
    }

    for (int i(0); i<spawn_count; ++i) {
      spawn_positions.push_back(emitter.PosRot);
    }
  }

  query_->Begin(ose::PrimitivesGenerated());
  transform_feedbacks_[current_tf()].BeginPoints();

  if (spawn_positions.size() > 0) {

    if (count_ == 0) {
      count_ = spawn_positions.size();
    }

    NoiseTexture::get().bind(ctx, 0);
    shader.noise_tex.        Set(0);
    math::vec2 time       (frame_time * 1000.0, total_time_ * 1000.0);
    math::vec2 life       (system.Life,            system.LifeVariance);
    math::vec2 pos_rot_variance  (system.PositionVariance,       system.RotationVariance);
    math::vec2 velocity   (system.Velocity,        system.VelocityVariance);
    math::vec2 rotation   (system.AngularVelocity, system.AngularVelocityVariance);

    shader.time.             Set(time);
    shader.life.             Set(life);
    shader.pos_rot_variance. Set(pos_rot_variance);
    shader.velocity.         Set(velocity);
    shader.rotation.         Set(rotation);

    // spawn new particles -----------------------------------------------------
    int index(0);

    while (index < spawn_positions.size()) {
      int count(std::min(50, (int)spawn_positions.size()-index));
      shader.spawn_count.Set(count);
      shader.transform.Set(std::vector<math::vec3>(spawn_positions.begin() + index, spawn_positions.begin() + index + count));
      ctx.gl.DrawArrays(ogl::PrimitiveType::Points, 0, 1);
      index += count;
    }
  }

  shader.spawn_count.Set(-1);

  // update existing particles -----------------------------------------------
  if (!first_draw) {
    Physics::get().bind_gravity_map(ctx, 0);
    math::vec3 dynamics(system.Mass, system.LinearDamping, system.AngularDamping);

    shader.gravity_map.Set(0);
    shader.projection. Set(ctx.projection_matrix);
    shader.dynamics.   Set(dynamics);

    ctx.gl.DrawTransformFeedback(
      ogl::PrimitiveType::Points, transform_feedbacks_[current_vb()]
    );
  }

  transform_feedbacks_[current_tf()].End();

  ogl::DefaultTransformFeedback().Bind();
  ctx.gl.Disable(ogl::Capability::RasterizerDiscard);

  query_->End(ose::PrimitivesGenerated());

  ogl::NoVertexArray().Bind();

  return count_;
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::draw_particles(RenderContext const& ctx) {

  particle_vaos_[current_tf()].Bind();

  ctx.gl.DrawTransformFeedback(
    ogl::PrimitiveType::Points, transform_feedbacks_[current_tf()]
  );

  ogl::NoVertexArray().Bind();
}

////////////////////////////////////////////////////////////////////////////////

int ParticleSystem::get_particle_count() const {
  return count_;
}

////////////////////////////////////////////////////////////////////////////////

}
