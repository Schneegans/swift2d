////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LISTENER_COMPONENT_HPP
#define SWIFT2D_LISTENER_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/openal.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A screen core represents a rectangular, virtual screen. Combined with a    //
// view core it defines the viewing frustum.                                  //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ListenerComponent;
typedef std::shared_ptr<ListenerComponent>       ListenerComponentPtr;
typedef std::shared_ptr<const ListenerComponent> ConstListenerComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL ListenerComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float Volume;

  // ----------------------------------------------------- constrution interface
  ListenerComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static ListenerComponentPtr create(Args&& ... a) {
    return std::make_shared<ListenerComponent>(a...);
  }

  // creates a copy from this
  ListenerComponentPtr create_copy() const {
    return std::make_shared<ListenerComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "ListenerComponent"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  oalplus::Listener listener_;


};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_LISTENER_COMPONENT_HPP
