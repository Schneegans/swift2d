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
#include <swift2d/graphics/Pipeline.hpp>

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
  : transform_feedbacks_()
  , particle_buffers_()
  , ping_(true)
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

void ParticleSystem::spawn(math::vec3 const& pos_rot, unsigned count) {
  new_particles_.push(pos_rot, count);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::spawn(std::vector<math::vec3> const& pos_rots) {
  new_particles_.push(pos_rots);
}

////////////////////////////////////////////////////////////////////////////////

int ParticleSystem::update_particles(ParticleSystemComponent::Serialized const& system,
  RenderContext const& ctx) {

  bool first_draw(false);

  // upload to GPU if neccessary
  if (particle_buffers_.empty()) {
    upload_to(ctx);
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
  double frame_time(ctx.pipeline->get_frame_time());

  // swap ping pong buffers
  ping_ = !ping_;

  ogl::Context::Enable(ogl::Capability::RasterizerDiscard);

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
  while(!new_particles_.empty()) {
    spawn_positions.push_back(new_particles_.pop());
  }

  ogl::Query::Activator qrya(*query_, ose::PrimitivesGenerated());
  ogl::TransformFeedback::Activator xfba(ogl::TransformFeedbackPrimitiveType::Points);

  if (spawn_positions.size() > 0) {

    if (count_ == 0) {
      count_ = spawn_positions.size();
    }

    NoiseTexture::get().bind(ctx, 0);
    shader.noise_tex.Set(0);
    math::vec2 time         (frame_time * 1000.0, ctx.pipeline->get_total_time() * 1000.0);
    math::vec2 life         (system.Life,            system.LifeVariance);
    math::vec2 velocity     (system.Velocity,        system.VelocityVariance);
    math::vec2 rotation     (system.AngularVelocity, system.AngularVelocityVariance);
    math::vec3 dir_pos_rot_variance(system.Rotation, system.PositionVariance, system.RotationVariance);

    shader.time.             Set(time);
    shader.life.             Set(life);
    shader.velocity.         Set(velocity);
    shader.rotation.         Set(rotation);
    shader.dir_pos_rot_variance.Set(dir_pos_rot_variance);

    // spawn new particles -----------------------------------------------------
    int index(0);

    while (index < spawn_positions.size()) {
      int count(std::min(50, (int)spawn_positions.size()-index));
      shader.spawn_count_it.Set(math::vec2i(count, index));
      shader.transform.Set(std::vector<math::vec3>(spawn_positions.begin() + index, spawn_positions.begin() + index + count));
      ogl::Context::DrawArrays(ogl::PrimitiveType::Points, 0, 1);
      index += count;
    }
  }

  shader.spawn_count_it.Set(math::vec2i(-1, 0));

  // update existing particles -----------------------------------------------
  if (!first_draw) {
    Physics::get().bind_gravity_map(ctx, 0);
    math::vec3 dynamics(system.Mass, system.LinearDamping, system.AngularDamping);

    shader.gravity_map.Set(0);
    shader.projection. Set(ctx.projection_matrix);
    shader.dynamics.   Set(dynamics);

    ogl::Context::DrawTransformFeedback(
      ogl::PrimitiveType::Points, transform_feedbacks_[current_vb()]
    );
  }

  xfba.Finish();
  ogl::DefaultTransformFeedback().Bind();
  ogl::Context::Disable(ogl::Capability::RasterizerDiscard);

  qrya.Finish();

  ogl::NoVertexArray().Bind();

  return count_;
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::draw_particles(RenderContext const& ctx) {

  particle_vaos_[current_tf()].Bind();

  ogl::Context::DrawTransformFeedback(
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
