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
  , count_(0)
  , query_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

ParticleSystem::~ParticleSystem() {
  if (query_) {
    delete query_;
  }
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
  new_particles_.push(std::make_pair(pos_rot, math::vec2()), count);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::spawn(math::vec3 const& pos_rot, math::vec2 const& vel, unsigned count) {
  new_particles_.push(std::make_pair(pos_rot, vel), count);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::spawn(std::vector<std::pair<math::vec3, math::vec2>> const& pos_rot_vel) {
  new_particles_.push(pos_rot_vel);
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
  std::vector<math::vec2> spawn_velocities;
  while(!new_particles_.empty()) {
    auto new_one(new_particles_.pop());
    spawn_positions.push_back(new_one.first);
    spawn_velocities.push_back(new_one.second);
  }

  ogl::Query::Activator qrya(*query_, ose::PrimitivesGenerated());
  ogl::TransformFeedback::Activator xfba(ogl::TransformFeedbackPrimitiveType::Points);

  if (spawn_positions.size() > 0) {

    if (count_ == 0) {
      count_ = spawn_positions.size();
    }

    NoiseTexture::get().bind(ctx, 0);
    math::vec2 time(frame_time * 1000.0, ctx.pipeline->get_total_time() * 1000.0);
    math::vec3 life_pos_var(system.Life, system.LifeVariance, system.PositionVariance);
    math::vec4 lin_ang_velocity(system.Velocity, system.VelocityVariance, system.AngularVelocity, system.AngularVelocityVariance);
    math::vec4 direction_rotation(system.Direction, system.DirectionVariance, system.Rotation, system.RotationVariance);

    shader.noise_tex.Set(0);
    shader.time.Set(time);
    shader.life_pos_var.Set(life_pos_var);
    shader.lin_ang_velocity.Set(lin_ang_velocity);
    shader.direction_rotation.Set(direction_rotation);

    // spawn new particles -----------------------------------------------------
    int index(0);

    while (index < spawn_positions.size()) {
      std::size_t count(std::min(50, (int)spawn_positions.size()-index));
      shader.spawn_count_it_collision_mode.Set(math::vec2i(count, index));
      shader.position.Set(count, (const math::vec3*)&spawn_positions[index]);
      shader.emitter_velocity.Set(count, (const math::vec2*)&spawn_velocities[index]);
      ogl::Context::DrawArrays(ogl::PrimitiveType::Points, 0, 1);
      index += count;
    }
  }

  shader.spawn_count_it_collision_mode.Set(math::vec2i(-1, static_cast<int>(system.CollisionMode)));

  // update existing particles -----------------------------------------------
  if (!first_draw) {
    Physics::get().bind_gravity_map(ctx, 0);
    math::vec3 dynamics(system.Mass, system.LinearDamping, system.AngularDamping);

    shader.gravity_map.Set(0);
    shader.projection.Set(ctx.projection_matrix);
    shader.dynamics.Set(dynamics);

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
