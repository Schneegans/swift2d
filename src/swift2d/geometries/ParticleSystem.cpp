////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/ParticleSystem.hpp>

#include <swift2d/physics/Physics.hpp>
#include <swift2d/components/ParticleSystemComponent.hpp>
#include <swift2d/materials/ParticleUpdateShader.hpp>
#include <swift2d/materials/TextureParticleShader.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

struct Particle {
  float type;
  math::vec2 pos;
  math::vec2 vel;
  float life;
};

////////////////////////////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem(ParticleSystemComponent* parent)
  : parent_(parent)
  , particles_to_spawn_(0.f)
  , transform_feedbacks_()
  , particle_buffers_()
  , ping_(true)
  , frame_time_(0.0)
  , total_time_(0.0) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::update(double time) {

  frame_time_ = time;
  total_time_ += time;

  if (parent_->Emitter()->Life() <= 0) {

    // free memory

  } else {

    // spawn new particles
    particles_to_spawn_ += parent_->Emitter()->Density() * time;
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::upload_to(RenderContext const& ctx) const {

  // allocate GPU resources
  for (int i(0); i<2; ++i) {
    transform_feedbacks_.push_back(oglplus::TransformFeedback());
    particle_buffers_.push_back(oglplus::Buffer());
    particle_vaos_.push_back(oglplus::VertexArray());

    particle_vaos_[i].Bind();
    particle_buffers_[i].Bind(oglplus::smart_enums::Array());

    std::vector<Particle> data(1000);
    data.front().type = 1;
    data.front().pos = math::vec2(0.f, 0.f);
    data.front().vel = math::vec2(0.0f, 0.0f);
    data.front().life = 1.f;
    oglplus::Buffer::Data(oglplus::smart_enums::Array(), data,
                          oglplus::smart_enums::DynamicDraw());

    oglplus::VertexArrayAttrib(0).Pointer(1, oglplus::DataType::Float, false, sizeof(Particle), (void const*)0);
    oglplus::VertexArrayAttrib(1).Pointer(2, oglplus::DataType::Float, false, sizeof(Particle), (void const*)4);
    oglplus::VertexArrayAttrib(2).Pointer(2, oglplus::DataType::Float, false, sizeof(Particle), (void const*)12);
    oglplus::VertexArrayAttrib(3).Pointer(1, oglplus::DataType::Float, false, sizeof(Particle), (void const*)20);
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::draw(RenderContext const& ctx) const {

  bool first_draw(false);

  // upload to GPU if neccessary
  if (particle_buffers_.empty()) {
    upload_to(ctx);
    first_draw = true;
  }

  ping_ = !ping_;

  // update particles ----------------------------------------------------------
  {
    ctx.gl.Enable(oglplus::Capability::RasterizerDiscard);
    auto shader(ParticleUpdateShader::instance());
    shader->use(ctx);

    particle_vaos_[current_vb()].Bind();
    transform_feedbacks_[current_tf()].Bind();
    particle_buffers_[current_tf()].BindBase(oglplus::smart_enums::TransformFeedback(), 0);

    oglplus::VertexArrayAttrib(0).Enable();
    oglplus::VertexArrayAttrib(1).Enable();
    oglplus::VertexArrayAttrib(2).Enable();
    oglplus::VertexArrayAttrib(3).Enable();

    transform_feedbacks_[current_tf()].BeginPoints();

    Physics::instance()->bind_gravity_map(ctx, 0);
    shader->set_uniform("gravity_map", 0);
    shader->set_uniform("projection", ctx.projection_matrix);
    shader->set_uniform("transform", parent_->WorldTransform());
    shader->set_uniform("time", math::vec2(frame_time_ * 1000.0, total_time_ * 1000.0));

    if (first_draw) {
      ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, 1);
    } else {
      ctx.gl.DrawTransformFeedback(oglplus::PrimitiveType::Points,
                                   transform_feedbacks_[current_vb()]);
    }

    transform_feedbacks_[current_tf()].End();

    oglplus::VertexArrayAttrib(0).Disable();
    oglplus::VertexArrayAttrib(1).Disable();
    oglplus::VertexArrayAttrib(2).Disable();
    oglplus::VertexArrayAttrib(3).Disable();
  }

  // draw particles ------------------------------------------------------------
  {
    ctx.gl.Disable(oglplus::Capability::RasterizerDiscard);
    auto shader(TextureParticleShader::instance());
    shader->use(ctx);
    shader->set_uniform("projection", ctx.projection_matrix);

    particle_vaos_[current_tf()].Bind();
    oglplus::VertexArrayAttrib(1).Enable();

    ctx.gl.DrawTransformFeedback(oglplus::PrimitiveType::Points,
                                   transform_feedbacks_[current_tf()]);

    oglplus::VertexArrayAttrib(1).Disable();

  }
}

////////////////////////////////////////////////////////////////////////////////

// void ParticleSystem::spawn() {

//   // index of newly spawned particle
//   int index(0);

//   // there is a location of a dead particle available
//   if (empty_positions_.size() > 0) {
//     index = empty_positions_.top();
//     empty_positions_.pop();

//   // create a new particle
//   } else {
//     index = positions_.size();
//     positions_.resize(index+1);
//     ages_.resize(index+1);
//     max_ages_.resize(index+1);
//     directions_.resize(index+1);
//     rots_.resize(index+1);
//     rot_speeds_.resize(index+1);
//   }

//   ages_[index] = 0.f;

//   parent_->Emitter()->spawn(
//     parent_->WorldTransform(),
//     positions_[index],
//     max_ages_[index],
//     directions_[index],
//     rots_[index],
//     rot_speeds_[index]
//   );
// }

////////////////////////////////////////////////////////////////////////////////

// void ParticleSystem::clear_all() {
//   positions_.clear();
//   directions_.clear();
//   ages_.clear();
//   max_ages_.clear();
//   rots_.clear();
//   rot_speeds_.clear();

//   while (!empty_positions_.empty()) {
//     empty_positions_.pop();
//   }
// }

////////////////////////////////////////////////////////////////////////////////

}
