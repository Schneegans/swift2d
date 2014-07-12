////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/GravitySourceComponent.hpp>

#include <swift2d/physics/Physics.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

GravitySourceComponent::GravitySourceComponent()
  : Density(10.f)
  , dirty_(true) {}

////////////////////////////////////////////////////////////////////////////////

void GravitySourceComponent::update(double time) {
  init();
}

////////////////////////////////////////////////////////////////////////////////

void GravitySourceComponent::init() const {
  if (dirty_) {
    dirty_ = false;
    Physics::instance()->add(this);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
