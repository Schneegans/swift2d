////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_RIGIDBODY_COMPONENT_HPP
#define SWIFT2D_RIGIDBODY_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class RigidbodyComponent;
typedef std::shared_ptr<RigidbodyComponent>       RigidbodyComponentPtr;
typedef std::shared_ptr<const RigidbodyComponent> ConstRigidbodyComponentPtr;

// -----------------------------------------------------------------------------
class RigidbodyComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  // ---------------------------------------------------------------- properties

  // ---------------------------------------------------- construction interface
  RigidbodyComponent();
  ~RigidbodyComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static RigidbodyComponentPtr create(Args&& ... a) {
    return std::make_shared<RigidbodyComponent>(a...);
  }

  // creates a copy from this
  RigidbodyComponentPtr create_copy() const {
    return std::make_shared<RigidbodyComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "RigidbodyComponent"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor) {
    TransformableComponent::accept(visitor);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_RIGIDBODY_COMPONENT_HPP
