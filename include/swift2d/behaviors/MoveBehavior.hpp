////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MOVE_BEHAVIOR_HPP
#define SWIFT2D_MOVE_BEHAVIOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/behaviors/Behavior.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class MoveBehavior;
typedef std::shared_ptr<MoveBehavior>       MoveBehaviorPtr;
typedef std::shared_ptr<const MoveBehavior> ConstMoveBehaviorPtr;

// -----------------------------------------------------------------------------
class MoveBehavior : public Behavior<SceneObject*> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  AnimatedFloat LinearSpeed;
  AnimatedFloat AngularSpeed;

  // ----------------------------------------------------- constrution interface
  MoveBehavior()
    : LinearSpeed()
    , AngularSpeed() {

    LinearSpeed.set(0);
    AngularSpeed.set(0);
  }

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static MoveBehaviorPtr create(Args&& ... a) {
    return std::make_shared<MoveBehavior>(a...);
  }

  // creates a copy from this
  MoveBehaviorPtr create_copy() const {
    return std::make_shared<MoveBehavior>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual void update(double time) {

    LinearSpeed.update(time);
    AngularSpeed.update(time);

    auto user_transform(get_user()->Transform.get());
    math::rotate(user_transform, AngularSpeed.get() * time);
    math::translate(user_transform, LinearSpeed.get() * time, 0);
    get_user()->Transform.set(user_transform);
  }

};

}

#endif  // SWIFT2D_MOVE_BEHAVIOR_HPP
