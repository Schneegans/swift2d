////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DYNAMIC_BODY_COMPONENT_HPP
#define SWIFT2D_DYNAMIC_BODY_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <iostream>

// forward declares ------------------------------------------------------------
class b2Body;

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class DynamicBodyComponent;
typedef std::shared_ptr<DynamicBodyComponent>       DynamicBodyComponentPtr;
typedef std::shared_ptr<const DynamicBodyComponent> ConstDynamicBodyComponentPtr;

// -----------------------------------------------------------------------------
class DynamicBodyComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float Radius;

  // ---------------------------------------------------- construction interface
  DynamicBodyComponent();
  ~DynamicBodyComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static DynamicBodyComponentPtr create(Args&& ... a) {
    return std::make_shared<DynamicBodyComponent>(a...);
  }

  // creates a copy from this
  DynamicBodyComponentPtr create_copy() const {
    return std::make_shared<DynamicBodyComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "DynamicBodyComponent"; }

  void       apply_global_force(math::vec2 const& val);
  void       apply_local_force(math::vec2 const& val);
  void       apply_torque(float val);

  void       set_linear_velocity(math::vec2 const& val);
  math::vec2 get_linear_velocity() const;
  void       set_angular_velocity(float val);
  float      get_angular_velocity() const;

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor) {
    Component::accept(visitor);
    visitor.add_member("Radius", Radius);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void init() const;

  mutable b2Body* body_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_DYNAMIC_BODY_COMPONENT_HPP
