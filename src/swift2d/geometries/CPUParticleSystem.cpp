////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/CPUParticleSystem.hpp>

#include <swift2d/materials/CPUParticleSystemShader.hpp>
#include <swift2d/components/ParticleSystemComponent.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

CPUParticleSystem::CPUParticleSystem(ParticleSystemComponent* parent)
  : parent_(parent)
  , particles_(nullptr)
  , pos_buf_(nullptr)
  , age_buf_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

CPUParticleSystem::~CPUParticleSystem() {
  if (particles_) delete particles_;
  if (pos_buf_)   delete pos_buf_;
  if (age_buf_)   delete age_buf_;
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::update(double time) {



  if (parent_->Emitter()->Life() <= 0) {
    clear_all();

  } else {
    for (int i(0); i<positions_.size(); ++i) {
      if (ages_[i] > 1.0f) {
        ages_[i] = -1.f;
        empty_positions_.push(i);
      } else if (ages_[i] >= 0.f) {
        positions_[i] += directions_[i] * time;
        ages_[i] += time / max_ages_[i];
      }
    }

    for (int i(0); i<parent_->Emitter()->Density(); ++i) {
      spawn();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::upload_to(RenderContext const& ctx) const {

  particles_ = new oglplus::VertexArray();
  pos_buf_   = new oglplus::Buffer();
  age_buf_   = new oglplus::Buffer();

  // bind the VAO for the particles
  particles_->Bind(); {

    // bind the VBO for the particle positions
    pos_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::VertexAttribArray(0).Setup<oglplus::Vec2f>().Enable();

    // bind the VBO for the particle ages
    age_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::VertexAttribArray(1).Setup<float>().Enable();
  }
  particles_->Unbind();
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::draw(RenderContext const& ctx) const {

  if (positions_.size() > 0) {

    // upload to GPU if neccessary
    if (!particles_) {
      upload_to(ctx);
    }

    CPUParticleSystemShader::instance()->use(ctx);
    parent_->Emitter()->Texture()->bind(ctx, 0);

    // Life
    // LifeVariance
    // Direction
    // DirectionVariance
    // StartRotation
    // EndRotation
    // RotationVariance
    // StartOpacity
    // EndOpacity
    // Colorize
    // StartColor
    // EndColor
    // Density
    // StartScale
    // EndScale

    CPUParticleSystemShader::instance()->set_uniform("diffuse", 0);
    CPUParticleSystemShader::instance()->set_uniform("projection", ctx.projection_matrix);

    if (parent_->Emitter()->WorldSpacePosition()) {
      CPUParticleSystemShader::instance()->set_uniform("transform", math::make_scale(math::get_scale(parent_->WorldTransform())));
    } else {
      CPUParticleSystemShader::instance()->set_uniform("transform", parent_->WorldTransform());
    }

    CPUParticleSystemShader::instance()->set_uniform("start_scale", parent_->Emitter()->StartScale());
    CPUParticleSystemShader::instance()->set_uniform("end_scale", parent_->Emitter()->EndScale());

    // update gpu data
    particles_->Bind();
    pos_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, positions_);

    age_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, ages_);

    // draw!
    ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, positions_.size());
  }
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::spawn() {

  int index(0);

  if (empty_positions_.size() > 0) {
    index = empty_positions_.top();
    empty_positions_.pop();
  } else {
    index = positions_.size();
    positions_.resize(index+1);
    ages_.resize(index+1);
    max_ages_.resize(index+1);
    directions_.resize(index+1);
  }

  math::vec2 scale(math::get_scale(parent_->WorldTransform()));

  // generate position
  if (parent_->Emitter()->WorldSpacePosition()) {
    math::vec2 pos(parent_->Emitter()->Position());
    positions_[index]     = (parent_->WorldTransform() * math::vec3(pos.x(), pos.y(), 1)).xy() ;
    positions_[index][0] /= scale.x();
    positions_[index][1] /= scale.y();
  } else {
    positions_[index] = parent_->Emitter()->Position();
  }

  // generate direction
  if (parent_->Emitter()->WorldSpaceDirection()) {
    directions_[index] = parent_->Emitter()->Direction();
  } else {
    math::vec2 dir(parent_->Emitter()->Direction());
    directions_[index]     =  (parent_->WorldTransform() * math::vec3(dir.x(), dir.y(), 0)).xy();
    directions_[index][0] +=  math::random::get(-0.1f, 0.1f);
    directions_[index][1] +=  math::random::get(-0.1f, 0.1f);
    directions_[index][0] /= scale.x();
    directions_[index][1] /= scale.y();
  }

  // generate life time
  ages_[index] = 0.f;
  float var(parent_->Emitter()->LifeVariance());
  max_ages_[index] = std::max(0.f, parent_->Emitter()->Life() + math::random::get(-var, var));
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::clear_all() {
  positions_.clear();
  directions_.clear();
  ages_.clear();
  max_ages_.clear();

  while (!empty_positions_.empty()) {
    empty_positions_.pop();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
