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
  : Density(10.f) {

  Physics::instance()->add(this);
}

////////////////////////////////////////////////////////////////////////////////

GravitySourceComponent::~GravitySourceComponent() {
  Physics::instance()->remove(this);
}

////////////////////////////////////////////////////////////////////////////////

}
