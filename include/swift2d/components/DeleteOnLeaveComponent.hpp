////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DELETE_ON_LEAVE_COMPONENT_HPP
#define SWIFT2D_DELETE_ON_LEAVE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/triggers/ShapeTrigger.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class DeleteOnLeaveComponent;
typedef std::shared_ptr<DeleteOnLeaveComponent>       DeleteOnLeaveComponentPtr;
typedef std::shared_ptr<const DeleteOnLeaveComponent> ConstDeleteOnLeaveComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL DeleteOnLeaveComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Signal<> on_delete;

  // ----------------------------------------------------- constrution interface
  DeleteOnLeaveComponent() {
    trigger_.on_leave.connect([&](){
      if (get_user()) {
        on_delete.emit();
        get_user()->detach();
      }

      return false;
    });
  }

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static DeleteOnLeaveComponentPtr create(Args&& ... a) {
    return std::make_shared<DeleteOnLeaveComponent>(a...);
  }

  // creates a copy from this
  DeleteOnLeaveComponentPtr create_copy() const {
    return std::make_shared<DeleteOnLeaveComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "DeleteOnLeaveComponent"; }

  void set_shapes(CircularShapePtr const& a, CircularShapePtr const& b) {
    trigger_.set_shapes(a, b);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ShapeTrigger trigger_;

};

}

#endif  // SWIFT2D_DELETE_ON_LEAVE_COMPONENT_HPP
