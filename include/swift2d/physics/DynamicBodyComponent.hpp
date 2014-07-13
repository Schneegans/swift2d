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

class StaticBodyComponent;

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
  Float Density;
  Float Friction;
  Float Restitution;
  Float LinearDamping;
  Float AngularDamping;

  // ------------------------------------------------------------------- signals
  Signal<StaticBodyComponent*> on_collision;

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
  void       apply_local_linear_impulse(math::vec2 const& val);
  void       apply_global_linear_impulse(math::vec2 const& val);
  void       apply_angular_impulse(float val);

  void       set_linear_velocity(math::vec2 const& val);
  math::vec2 get_linear_velocity();
  void       set_angular_velocity(float val);
  float      get_angular_velocity();

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor) {
    Component::accept(visitor);
    visitor.add_member("Radius", Radius);
    visitor.add_member("Density", Density);
    visitor.add_member("Friction", Friction);
    visitor.add_member("Restitution", Restitution);
    visitor.add_member("LinearDamping", LinearDamping);
    visitor.add_member("AngularDamping", AngularDamping);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void init();

  mutable b2Body* body_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_DYNAMIC_BODY_COMPONENT_HPP
