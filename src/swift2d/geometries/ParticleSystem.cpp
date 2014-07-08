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
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem(ParticleSystemComponent* parent)
  : parent_(parent)
  , particles_to_spawn_(0.f)
  , particles_(nullptr)
  , pos_buf_(nullptr)
  , age_buf_(nullptr)
  , rot_buf_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

ParticleSystem::~ParticleSystem() {
  if (particles_) delete particles_;
  if (pos_buf_)   delete pos_buf_;
  if (age_buf_)   delete age_buf_;
  if (rot_buf_)   delete rot_buf_;
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::update(double time) {

  if (parent_->Emitter()->Life() <= 0) {

    // free memory
    clear_all();

  } else {

    // update active particles
    for (int i(0); i<positions_.size(); ++i) {
      if (ages_[i] > 1.0f) {
        // dead - overwrite with new particles
        ages_[i] = -1.f;
        empty_positions_.push(i);
      } else if (ages_[i] >= 0.f) {
        // update movement
        positions_[i] += directions_[i] * time;
        ages_[i] += time / max_ages_[i];
        rots_[i] += time * rot_speeds_[i];
      }
    }

    // spawn new particles
    particles_to_spawn_ += parent_->Emitter()->Density() * time;

    while (particles_to_spawn_ > 1.f) {
      particles_to_spawn_ -= 1.f;
      spawn();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::upload_to(RenderContext const& ctx) const {

  // allocate GPU resources
  particles_ = new oglplus::VertexArray();
  pos_buf_   = new oglplus::Buffer();
  age_buf_   = new oglplus::Buffer();
  rot_buf_   = new oglplus::Buffer();

  // bind the VAO for the particles
  particles_->Bind(); {

    // bind the VBO for the particle positions
    pos_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::VertexArrayAttrib(0).Setup<oglplus::Vec2f>().Enable();

    // bind the VBO for the particle ages
    age_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::VertexArrayAttrib(1).Setup<float>().Enable();

    // bind the VBO for the particle rotations
    rot_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::VertexArrayAttrib(2).Setup<float>().Enable();
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::draw(RenderContext const& ctx) const {

  if (positions_.size() > 0) {

    // upload to GPU if neccessary
    if (!particles_) {
      upload_to(ctx);
    }

    // update gpu data
    particles_->Bind();
    pos_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, positions_);

    age_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, ages_);

    rot_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, rots_);

    // draw
    parent_->Emitter()->draw(ctx, parent_->WorldTransform(), positions_.size());
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::spawn() {


  // index of newly spawned particle
  int index(0);

  // there is a location of a dead particle available
  if (empty_positions_.size() > 0) {
    index = empty_positions_.top();
    empty_positions_.pop();

  // create a new particle
  } else {
    index = positions_.size();
    positions_.resize(index+1);
    ages_.resize(index+1);
    max_ages_.resize(index+1);
    directions_.resize(index+1);
    rots_.resize(index+1);
    rot_speeds_.resize(index+1);
  }

  ages_[index] = 0.f;

  parent_->Emitter()->spawn(
    parent_->WorldTransform(),
    positions_[index],
    max_ages_[index],
    directions_[index],
    rots_[index],
    rot_speeds_[index]
  );
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::clear_all() {
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
