////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TIMED_DELETE_COMPONENT_HPP
#define SWIFT2D_TIMED_DELETE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class TimedDeleteComponent;
typedef std::shared_ptr<TimedDeleteComponent>       TimedDeleteComponentPtr;
typedef std::shared_ptr<const TimedDeleteComponent> ConstTimedDeleteComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL TimedDeleteComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float Life;
  Signal<> on_delete;

  // ----------------------------------------------------- constrution interface
  TimedDeleteComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static TimedDeleteComponentPtr create(Args&& ... a) {
    return std::make_shared<TimedDeleteComponent>(a...);
  }

  // creates a copy from this
  TimedDeleteComponentPtr create_copy() const {
    return std::make_shared<TimedDeleteComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "TimedDeleteComponent"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);
};

}

#endif  // SWIFT2D_TIMED_DELETE_COMPONENT_HPP
