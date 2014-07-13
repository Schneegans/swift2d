////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/behaviors/MoveBehavior.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

MoveBehavior::MoveBehavior()
  : LinearSpeed()
  , AngularSpeed() {

  LinearSpeed.set(0);
  AngularSpeed.set(0);
}

////////////////////////////////////////////////////////////////////////////////

void MoveBehavior::update(double time) {
  auto user_transform(get_user()->Transform.get());
  math::rotate(user_transform, AngularSpeed.get() * time);
  math::translate(user_transform, LinearSpeed.get() * time, 0);
  get_user()->Transform.set(user_transform);
}

////////////////////////////////////////////////////////////////////////////////

void MoveBehavior::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_member("LinearSpeed", LinearSpeed);
  visitor.add_member("AngularSpeed", AngularSpeed);
}

////////////////////////////////////////////////////////////////////////////////

}
