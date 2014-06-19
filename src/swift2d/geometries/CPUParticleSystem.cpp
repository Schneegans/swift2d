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
  , particles_to_spawn_(0.f)
  , particles_(nullptr)
  , pos_buf_(nullptr)
  , age_buf_(nullptr)
  , rot_buf_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

CPUParticleSystem::~CPUParticleSystem() {
  if (particles_) delete particles_;
  if (pos_buf_)   delete pos_buf_;
  if (age_buf_)   delete age_buf_;
  if (rot_buf_)   delete rot_buf_;
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
        rots_[i] += time * rot_speeds_[i];
      }
    }

    particles_to_spawn_ += parent_->Emitter()->Density() * time;

    while (particles_to_spawn_ > 1.f) {
      particles_to_spawn_ -= 1.f;
      spawn();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::upload_to(RenderContext const& ctx) const {

  particles_ = new oglplus::VertexArray();
  pos_buf_   = new oglplus::Buffer();
  age_buf_   = new oglplus::Buffer();
  rot_buf_   = new oglplus::Buffer();

  // bind the VAO for the particles
  particles_->Bind(); {

    // bind the VBO for the particle positions
    pos_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::VertexAttribArray(0).Setup<oglplus::Vec2f>().Enable();

    // bind the VBO for the particle ages
    age_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::VertexAttribArray(1).Setup<float>().Enable();

    // bind the VBO for the particle rotations
    rot_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::VertexAttribArray(2).Setup<float>().Enable();
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

    auto emitter(parent_->Emitter());

    CPUParticleSystemShader::instance()->use(ctx);
    emitter->Texture()->bind(ctx, 0);

    CPUParticleSystemShader::instance()->set_uniform("diffuse", 0);
    CPUParticleSystemShader::instance()->set_uniform("projection", ctx.projection_matrix);

    if (emitter->WorldSpacePosition()) {
      CPUParticleSystemShader::instance()->set_uniform("transform", math::make_scale(math::get_scale(parent_->WorldTransform())));
    } else {
      CPUParticleSystemShader::instance()->set_uniform("transform", parent_->WorldTransform());
    }

    CPUParticleSystemShader::instance()->set_uniform("start_scale", emitter->StartScale());
    CPUParticleSystemShader::instance()->set_uniform("end_scale", emitter->EndScale());

    CPUParticleSystemShader::instance()->set_uniform("start_color", emitter->StartColor().vec3());
    CPUParticleSystemShader::instance()->set_uniform("end_color", emitter->EndColor().vec3());

    CPUParticleSystemShader::instance()->set_uniform("start_opacity", emitter->StartOpacity());
    CPUParticleSystemShader::instance()->set_uniform("end_opacity", emitter->EndOpacity());

    bool enable_rotation(emitter->RotationSpeed() != 0.f || emitter->RotationSpeedVariance() != 0.f || emitter->Rotation() != 0.f || emitter->RotationVariance() != 0.f);
    CPUParticleSystemShader::instance()->set_uniform("enable_rotation", (int)enable_rotation);

    if (emitter->BlendAdditive()) {
      ctx.gl.BlendFunc(oglplus::BlendFn::SrcAlpha, oglplus::BlendFn::One);
    }

    // update gpu data
    particles_->Bind();
    pos_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, positions_);

    age_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, ages_);

    rot_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, rots_);

    // draw!
    ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, positions_.size());

    if (emitter->BlendAdditive()) {
      ctx.gl.BlendFunc(oglplus::BlendFn::SrcAlpha, oglplus::BlendFn::OneMinusSrcAlpha);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::spawn() {

  auto emitter(parent_->Emitter());

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
    rots_.resize(index+1);
    rot_speeds_.resize(index+1);
  }

  math::vec2 scale(math::get_scale(parent_->WorldTransform()));

  // generate position
  if (emitter->WorldSpacePosition()) {
    math::vec2 pos(emitter->Position());
    positions_[index]     = (parent_->WorldTransform() * math::vec3(pos.x(), pos.y(), 1)).xy() ;
    positions_[index][0] /= scale.x();
    positions_[index][1] /= scale.y();
  } else {
    positions_[index] = emitter->Position();
  }

  // generate direction
  if (emitter->WorldSpaceDirection()) {
    directions_[index] = emitter->Direction();
  } else {
    math::vec2 dir(emitter->Direction());
    directions_[index]     =  (parent_->WorldTransform() * math::vec3(dir.x(), dir.y(), 0)).xy();
    directions_[index][0] /= scale.x();
    directions_[index][1] /= scale.y();
  }

  auto direction_variance = math::make_rotate(math::random::get(-emitter->DirectionVariance(), emitter->DirectionVariance()));
  directions_[index] = (direction_variance * math::vec3(directions_[index][0], directions_[index][1], 0.0f)).xy();


  // generate rotation
  rots_[index] = emitter->Rotation() + math::random::get(-emitter->RotationVariance(), emitter->RotationVariance());
  rot_speeds_[index] = emitter->RotationSpeed() + math::random::get(-emitter->RotationSpeedVariance(), emitter->RotationSpeedVariance());

  // generate life time
  ages_[index] = 0.f;
  float var(emitter->LifeVariance());
  max_ages_[index] = std::max(0.f, emitter->Life() + math::random::get(-var, var));
}

////////////////////////////////////////////////////////////////////////////////

void CPUParticleSystem::clear_all() {
  positions_.clear();
  directions_.clear();
  ages_.clear();
  max_ages_.clear();
  rots_.clear();
  rot_speeds_.clear();

  while (!empty_positions_.empty()) {
    empty_positions_.pop();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
