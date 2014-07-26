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

ParticleSystem::ParticleSystem()
  : particles_to_spawn_()
  , transform_feedbacks_()
  , particle_buffers_()
  , ping_(true)
  , frame_time_(0.0)
  , total_time_(0.0) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::update(std::unordered_set<ParticleEmitterComponentPtr> const& emitters,
                            double time) {
  frame_time_ = time;
  total_time_ += time;

  for (auto const& emitter: emitters) {
    particles_to_spawn_[emitter] += emitter->Density() * frame_time_;
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::upload_to(RenderContext const& ctx) {

  // allocate GPU resources
  for (int i(0); i<2; ++i) {
    transform_feedbacks_.push_back(oglplus::TransformFeedback());
    particle_buffers_.push_back(oglplus::Buffer());
    particle_vaos_.push_back(oglplus::VertexArray());

    particle_vaos_[i].Bind();
    particle_buffers_[i].Bind(oglplus::smart_enums::Array());

    std::vector<Particle> data(1000);
    data.front().pos =  math::vec2(0.f, 0.f);
    data.front().vel =  math::vec2(0.f, 0.f);
    data.front().life = math::vec2(0.f, 0.f);
    oglplus::Buffer::Data(oglplus::smart_enums::Array(), data,
                          oglplus::smart_enums::DynamicDraw());

    oglplus::VertexArrayAttrib(0).Pointer(2, oglplus::DataType::Float, false, sizeof(Particle), (void const*)0);
    oglplus::VertexArrayAttrib(1).Pointer(2, oglplus::DataType::Float, false, sizeof(Particle), (void const*)8);
    oglplus::VertexArrayAttrib(2).Pointer(2, oglplus::DataType::Float, false, sizeof(Particle), (void const*)16);
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::update_particles(std::unordered_set<ParticleEmitterComponentPtr> const& emitters,
                                      RenderContext const& ctx) {

  bool first_draw(false);

  // upload to GPU if neccessary
  if (particle_buffers_.empty()) {
    upload_to(ctx);
    first_draw = true;
  }

  ping_ = !ping_;

  ctx.gl.Enable(oglplus::Capability::RasterizerDiscard);

  particle_vaos_      [current_vb()].Bind();
  transform_feedbacks_[current_tf()].Bind();
  particle_buffers_   [current_tf()].BindBase(oglplus::smart_enums::TransformFeedback(), 0);


  auto shader(ParticleUpdateShader::instance());
  shader->use(ctx);

  oglplus::VertexArrayAttrib(0).Enable();
  oglplus::VertexArrayAttrib(1).Enable();
  oglplus::VertexArrayAttrib(2).Enable();

  transform_feedbacks_[current_tf()].BeginPoints();

  // spawn new particles -------------------------------------------------------
  for (auto const& emitter: emitters) {
    int spawn_count(particles_to_spawn_[emitter]);
    particles_to_spawn_[emitter] -= spawn_count;
    shader->set_uniform("spawn_count",  spawn_count);
    shader->set_uniform("transform",    emitter->WorldTransform());
    ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, 1);
  }

  // update existing particles -------------------------------------------------
  if (!first_draw) {
    Physics::instance()->bind_gravity_map(ctx, 0);
    shader->set_uniform("spawn_count", -1);
    shader->set_uniform("time",         math::vec2(frame_time_ * 1000.0, total_time_ * 1000.0));
    shader->set_uniform("gravity_map",  0);
    shader->set_uniform("projection",   ctx.projection_matrix);
    shader->set_uniform("mass",         -0.5f);

    ctx.gl.DrawTransformFeedback(oglplus::PrimitiveType::Points, transform_feedbacks_[current_vb()]);
  }

  transform_feedbacks_[current_tf()].End();

  oglplus::VertexArrayAttrib(0).Disable();
  oglplus::VertexArrayAttrib(1).Disable();
  oglplus::VertexArrayAttrib(2).Disable();

  ctx.gl.Disable(oglplus::Capability::RasterizerDiscard);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::draw_particles(RenderContext const& ctx) {
  particle_vaos_[current_tf()].Bind();
  oglplus::VertexArrayAttrib(0).Enable();
  ctx.gl.DrawTransformFeedback(oglplus::PrimitiveType::Points, transform_feedbacks_[current_tf()]);
  oglplus::VertexArrayAttrib(0).Disable();
}

////////////////////////////////////////////////////////////////////////////////

}
