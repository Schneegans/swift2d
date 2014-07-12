////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/StaticBodyComponent.hpp>

#include <swift2d/physics/Physics.hpp>
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/math/transformations.hpp>

#include <Box2D/Box2D.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

StaticBodyComponent::StaticBodyComponent()
  : body_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

StaticBodyComponent::~StaticBodyComponent() {
}

////////////////////////////////////////////////////////////////////////////////

void StaticBodyComponent::update(double time) {
  if (!body_) {
    body_ = Physics::instance()->add(this);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
