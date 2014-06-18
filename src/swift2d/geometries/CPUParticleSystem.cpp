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
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

CPUParticleSystem::CPUParticleSystem()
  : particles_(nullptr)
  , pos_buf_(nullptr)
  , age_buf_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

CPUParticleSystem::~CPUParticleSystem() {
  if (particles_) delete particles_;
  if (pos_buf_)   delete pos_buf_;
  if (age_buf_)   delete age_buf_;
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::update(double time, math::mat3 const& object_transform) {

  Life.update(time);
  Density.update(time);

  if (Life() <= 0) {
    clear_all();

  } else {
    for (int i(0); i<positions_.size(); ++i) {
      if (ages_[i] > 1.0f) {
        ages_[i] = -1.f;
        empty_positions_.push(i);
      } else if (ages_[i] >= 0.f) {
        positions_[i] += directions_[i] * time;
        ages_[i] += time / Life();
      }
    }

    for (int i(0); i<Density(); ++i) {
      spawn(object_transform);
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

void CPUParticleSystem::draw(RenderContext const& ctx, math::mat3 const& object_transform) const {

  if (positions_.size() > 0) {

    // upload to GPU if neccessary
    if (!particles_) {
      upload_to(ctx);
    }

    Texture()->bind(ctx, 0);

    CPUParticleSystemShader::instance()->use(ctx);
    CPUParticleSystemShader::instance()->set_uniform("diffuse", 0);
    CPUParticleSystemShader::instance()->set_uniform("projection", ctx.projection_matrix);

    if (InWorldSpace()) {
      CPUParticleSystemShader::instance()->set_uniform("transform", math::make_scale(math::get_scale(object_transform)));
    } else {
      CPUParticleSystemShader::instance()->set_uniform("transform", object_transform);
    }

    particles_->Bind();
    pos_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, positions_);

    age_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, ages_);

    ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, positions_.size());
  }
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::spawn(math::mat3 const& object_transform) {

  int index(0);

  if (empty_positions_.size() > 0) {
    index = empty_positions_.top();
    empty_positions_.pop();
  } else {
    index = positions_.size();
    positions_.resize(index+1);
    ages_.resize(index+1);
    directions_.resize(index+1);
  }

  if (InWorldSpace()) {

    math::vec2 scale(math::get_scale(object_transform));

    positions_[index]     =  math::get_translate(object_transform);
    positions_[index][0] /= scale.x();
    positions_[index][1] /= scale.y();

    directions_[index]     =  (object_transform * math::vec3(-10, 0, 0)).xy();
    directions_[index][0] +=  math::random::get(-0.1f, 0.1f);
    directions_[index][1] +=  math::random::get(-0.1f, 0.1f);
    directions_[index][0] /= scale.x();
    directions_[index][1] /= scale.y();

  } else {
    positions_[index]  = math::vec2(0.f, 0.f);
    directions_[index] = math::vec2(-10, 0);
  }

  ages_[index] = 0.f;
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::clear_all() {
  positions_.clear();
  directions_.clear();
  ages_.clear();

  while (!empty_positions_.empty()) {
    empty_positions_.pop();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
