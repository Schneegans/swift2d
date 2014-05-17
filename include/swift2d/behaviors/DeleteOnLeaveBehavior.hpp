////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DELETE_ON_LEAVE_BEHAVIOR_HPP
#define SWIFT2D_DELETE_ON_LEAVE_BEHAVIOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/behaviors/Behavior.hpp>
#include <swift2d/triggers/ShapeTrigger.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class DeleteOnLeaveBehavior;
typedef std::shared_ptr<DeleteOnLeaveBehavior>       DeleteOnLeaveBehaviorPtr;
typedef std::shared_ptr<const DeleteOnLeaveBehavior> ConstDeleteOnLeaveBehaviorPtr;

// -----------------------------------------------------------------------------
class DeleteOnLeaveBehavior : public Behavior<SceneObject*> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- constrution interface
  DeleteOnLeaveBehavior() {
    int callback;
    callback = trigger_.on_leave.connect([&](){
      if (get_user()) {
        trigger_.on_leave.disconnect(callback);
        get_user()->detach();
      }
    });
  }

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static DeleteOnLeaveBehaviorPtr create(Args&& ... a) {
    return std::make_shared<DeleteOnLeaveBehavior>(a...);
  }

  // creates a copy from this
  DeleteOnLeaveBehaviorPtr create_copy() const {
    return std::make_shared<DeleteOnLeaveBehavior>(*this);
  }

  // ------------------------------------------------------------ public methods
  void set_shapes(CircularShapePtr const& a, CircularShapePtr const& b) {
    trigger_.set_shapes(a, b);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ShapeTrigger trigger_;

};

}

#endif  // SWIFT2D_DELETE_ON_LEAVE_BEHAVIOR_HPP
