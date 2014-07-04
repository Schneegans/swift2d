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
#include <swift2d/components/Component.hpp>
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
class MoveBehavior : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float LinearSpeed;
  Float AngularSpeed;

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
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "MoveBehavior"; }

  virtual void update(double time) {
    auto user_transform(get_user()->Transform.get());
    math::rotate(user_transform, AngularSpeed.get() * time);
    math::translate(user_transform, LinearSpeed.get() * time, 0);
    get_user()->Transform.set(user_transform);
  }

  virtual void accept(SavableObjectVisitor& visitor) {
    Component::accept(visitor);
    visitor.add_member("LinearSpeed", LinearSpeed);
    visitor.add_member("AngularSpeed", AngularSpeed);
  }

};

}

#endif  // SWIFT2D_MOVE_BEHAVIOR_HPP
