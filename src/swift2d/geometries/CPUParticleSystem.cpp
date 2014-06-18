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

void CPUParticleSystem::update(double time) {
  static int c(0);
  if (positions_.size() < 100 && (c++)%20==0) {
    positions_.push_back(math::vec2(0.f, 0.f));
    ages_.push_back(0);
  }

  for (int i(0); i<positions_.size(); ++i) {
    positions_[i] += math::vec2(0.2, 0);
    ages_[i] += time;
  }
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::upload_to(RenderContext const& ctx) const {
  ctx.gl.PointSize(20);

  particles_ = new oglplus::VertexArray();
  pos_buf_   = new oglplus::Buffer();
  age_buf_   = new oglplus::Buffer();

  // bind the VAO for the particles
  particles_->Bind(); {

    // bind the VBO for the particle positions
    positions_.reserve(100);
    pos_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, positions_);
    oglplus::VertexAttribArray(0).Setup<oglplus::Vec2f>().Enable();

    // bind the VBO for the particle ages
    ages_.reserve(100);
    age_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, ages_);
    oglplus::VertexAttribArray(1).Setup<float>().Enable();
  }
  particles_->Unbind();
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::draw(RenderContext const& ctx, math::mat3 const& object_transform) const {

  // upload to GPU if neccessary
  if (!particles_) {
    upload_to(ctx);
  }

  CPUParticleSystemShader::instance()->use(ctx);
  CPUParticleSystemShader::instance()->set_uniform("projection", ctx.projection_matrix);
  CPUParticleSystemShader::instance()->set_uniform("transform", object_transform);

  particles_->Bind();
  pos_buf_->Bind(oglplus::Buffer::Target::Array);
  oglplus::Buffer::Data(oglplus::Buffer::Target::Array, positions_);

  age_buf_->Bind(oglplus::Buffer::Target::Array);
  oglplus::Buffer::Data(oglplus::Buffer::Target::Array, ages_);

  ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, positions_.size());
}

////////////////////////////////////////////////////////////////////////////////

}
