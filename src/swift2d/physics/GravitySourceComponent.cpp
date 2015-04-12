////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/GravitySourceComponent.hpp>

#include <swift2d/physics/Physics.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

GravitySourceComponent::GravitySourceComponent()
  : Mass(10.f)
  , CollisionRadius(0.f) {

  Physics::get().add(this);
}

////////////////////////////////////////////////////////////////////////////////

GravitySourceComponent::~GravitySourceComponent() {
  Physics::get().remove(this);
}

////////////////////////////////////////////////////////////////////////////////

void GravitySourceComponent::serialize(SerializedScenePtr& scene) const {
  scene->gravity_sources.push_back(create_copy());
}

////////////////////////////////////////////////////////////////////////////////

void GravitySourceComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Mass", Mass);
  visitor.add_member("CollisionRadius", CollisionRadius);
}

////////////////////////////////////////////////////////////////////////////////

}
