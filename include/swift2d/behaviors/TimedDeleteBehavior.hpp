////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TIMED_DELETE_BEHAVIOR_HPP
#define SWIFT2D_TIMED_DELETE_BEHAVIOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class TimedDeleteBehavior;
typedef std::shared_ptr<TimedDeleteBehavior>       TimedDeleteBehaviorPtr;
typedef std::shared_ptr<const TimedDeleteBehavior> ConstTimedDeleteBehaviorPtr;

// -----------------------------------------------------------------------------
class TimedDeleteBehavior : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float Life;
  Signal<> on_delete;

  // ----------------------------------------------------- constrution interface
  TimedDeleteBehavior();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static TimedDeleteBehaviorPtr create(Args&& ... a) {
    return std::make_shared<TimedDeleteBehavior>(a...);
  }

  // creates a copy from this
  TimedDeleteBehaviorPtr create_copy() const {
    return std::make_shared<TimedDeleteBehavior>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "TimedDeleteBehavior"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);
};

}

#endif  // SWIFT2D_TIMED_DELETE_BEHAVIOR_HPP
