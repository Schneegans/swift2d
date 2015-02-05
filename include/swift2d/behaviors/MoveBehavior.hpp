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
class SWIFT_DLL MoveBehavior : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float LinearSpeed;
  Float AngularSpeed;

  // ----------------------------------------------------- constrution interface
  MoveBehavior();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static MoveBehaviorPtr create(Args&& ... a) {
    return std::make_shared<MoveBehavior>(a...);
  }

  // creates a copy from this
  MoveBehaviorPtr create_copy() const {
    return std::make_shared<MoveBehavior>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "MoveBehavior"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);
};

}

#endif  // SWIFT2D_MOVE_BEHAVIOR_HPP
