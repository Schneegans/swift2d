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

#include <swift2d/components/MoveComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

MoveComponent::MoveComponent()
  : LinearSpeed(math::vec2(0.f, 0.f))
  , LinearDamping(0)
  , AngularSpeed(0)
  , AngularDamping(0)
  , IgnoreParentRotation(false)
  , rotation_(0.f) {}

////////////////////////////////////////////////////////////////////////////////

void MoveComponent::update(double time) {
  auto user_transform(get_user()->Transform.get());

  if (IgnoreParentRotation() && get_user()->Parent()) {
    rotation_ += AngularSpeed.get() * time;
    math::set_rotation(user_transform, rotation_ - get_user()->Parent()->get_world_rotation());
  } else {
    math::rotate(user_transform, AngularSpeed.get() * time);
  }
  math::translate(user_transform, LinearSpeed.get() * time);
  get_user()->Transform.set(user_transform);

  LinearSpeed = LinearSpeed() - LinearSpeed()*time*LinearDamping();
  AngularSpeed = AngularSpeed() - AngularSpeed()*time*AngularDamping();
}

////////////////////////////////////////////////////////////////////////////////

void MoveComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_member("LinearSpeed", LinearSpeed);
  visitor.add_member("LinearDamping", LinearDamping);
  visitor.add_member("AngularSpeed", AngularSpeed);
  visitor.add_member("AngularDamping", AngularDamping);
  visitor.add_member("IgnoreParentRotation", IgnoreParentRotation);
}

////////////////////////////////////////////////////////////////////////////////

}
