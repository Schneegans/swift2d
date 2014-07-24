////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/ParticleSystem.hpp>

#include <swift2d/components/ParticleSystemComponent.hpp>
#include <swift2d/materials/ParticleUpdateShader.hpp>
#include <swift2d/materials/TextureParticleShader.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem(ParticleSystemComponent* parent)
  : parent_(parent)
  , particles_to_spawn_(0.f)
  , transform_feedbacks_()
  , particle_buffers_()
  , ping_(true) {}
  // , particles_(nullptr)
  // , pos_buf_(nullptr)
  // , age_buf_(nullptr)
  // , rot_buf_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

ParticleSystem::~ParticleSystem() {
  // if (particles_) delete particles_;
  // if (pos_buf_)   delete pos_buf_;
  // if (age_buf_)   delete age_buf_;
  // if (rot_buf_)   delete rot_buf_;
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::update(double time) {

  if (parent_->Emitter()->Life() <= 0) {

    // free memory
    // clear_all();

  } else {

    // // update active particles
    // for (int i(0); i<positions_.size(); ++i) {
    //   if (ages_[i] > 1.0f) {
    //     // dead - overwrite with new particles
    //     ages_[i] = -1.f;
    //     empty_positions_.push(i);
    //   } else if (ages_[i] >= 0.f) {
    //     // update movement
    //     positions_[i] += directions_[i] * time;
    //     ages_[i] += time / max_ages_[i];
    //     rots_[i] += time * rot_speeds_[i];
    //   }
    // }

    // spawn new particles
    particles_to_spawn_ += parent_->Emitter()->Density() * time;

    // while (particles_to_spawn_ > 1.f) {
    //   particles_to_spawn_ -= 1.f;
    //   spawn();
    // }
  }
}

////////////////////////////////////////////////////////////////////////////////

struct Particle {
  float type;
  math::vec2 pos;
  math::vec2 vel;
  float life;
};

void ParticleSystem::upload_to(RenderContext const& ctx) const {

  // allocate GPU resources
  for (int i(0); i<2; ++i) {
    // transform_feedbacks_.push_back(oglplus::TransformFeedback());
    particle_buffers_.push_back(oglplus::Buffer());
    particle_vaos_.push_back(oglplus::VertexArray());

    particle_vaos_[i].Bind();
    particle_buffers_[i].Bind(oglplus::smart_enums::Array());

    oglplus::VertexArrayAttrib(0).Pointer(1, oglplus::DataType::Float, false, sizeof(Particle), (void const*)0);
    oglplus::VertexArrayAttrib(1).Pointer(2, oglplus::DataType::Float, false, sizeof(Particle), (void const*)4);
    oglplus::VertexArrayAttrib(2).Pointer(2, oglplus::DataType::Float, false, sizeof(Particle), (void const*)12);
    oglplus::VertexArrayAttrib(3).Pointer(1, oglplus::DataType::Float, false, sizeof(Particle), (void const*)20);

    std::vector<Particle> data(1);
    data.back().type = 1.f;
    data.back().pos = math::vec2(0.f, 0.f);
    data.back().vel = math::vec2(0.01f, 0.f);
    data.back().life = 10.f;
    oglplus::Buffer::Data(oglplus::smart_enums::Array(), data);

    oglplus::NoVertexArray().Bind();

    // transform_feedbacks_[i].Bind();
    // particle_buffers_[i].Bind(oglplus::smart_enums::TransformFeedback());
    // particle_buffers_[i].BindBase(oglplus::smart_enums::TransformFeedback(), 0);
    // default_transform_feedback_.Bind();
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
    // ctx.gl.Enable(oglplus::Capability::RasterizerDiscard);
    // auto shader(ParticleUpdateShader::instance());
    auto shader(TextureParticleShader::instance());
    shader->use(ctx);

    particle_vaos_[current_vb()].Bind();
    // transform_feedbacks_[current_tf()].Bind();

    oglplus::VertexArrayAttrib(0).Enable();
    oglplus::VertexArrayAttrib(1).Enable();
    oglplus::VertexArrayAttrib(2).Enable();
    oglplus::VertexArrayAttrib(3).Enable();

  //   transform_feedbacks_[current_tf()].BeginPoints();

  //   // if (first_draw) {
      ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, 1);
  //   // } else {
  //   //   ctx.gl.DrawTransformFeedback(oglplus::PrimitiveType::Points,
  //   //                                transform_feedbacks_[current_vb()]);
  //   // }

  //   transform_feedbacks_[current_tf()].End();

    oglplus::VertexArrayAttrib(0).Disable();
    oglplus::VertexArrayAttrib(1).Disable();
    oglplus::VertexArrayAttrib(2).Disable();
    oglplus::VertexArrayAttrib(3).Disable();

    // default_transform_feedback_.Bind();
  }


  // draw particles ------------------------------------------------------------
  {
    // ctx.gl.Disable(oglplus::Capability::RasterizerDiscard);
    // auto shader(TextureParticleShader::instance());
    // shader->use(ctx);

  //   oglplus::VertexArrayAttrib(0).Pointer(2, oglplus::DataType::Float, false, sizeof(Particle), (void const*)4).Enable();

  //   // particle_buffers_[current_tf()].Bind(oglplus::Buffer::Target::Array);
  //   // ctx.gl.DrawTransformFeedback(oglplus::PrimitiveType::Points,
  //   //                                transform_feedbacks_[current_tf()]);

  //   oglplus::VertexArrayAttrib(0).Disable();

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
