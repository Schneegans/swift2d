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
#include <swift2d/physics/CollisionShape.hpp>

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
class SWIFT_DLL DynamicBodyComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  CollisionShapeProperty  Shape;
  Float                   Mass;
  Float                   Friction;
  Float                   Restitution;
  Float                   LinearDamping;
  Float                   AngularDamping;
  Float                   GravityScale;
  Bool                    IsBullet;
  Bool                    FixedRotation;
  Bool                    Sleep;

  Int16                   Group;
  UInt16                  Mask;
  UInt16                  Category;

  // ------------------------------------------------------------------- signals
  Signal<StaticBodyComponent*, math::vec2>  start_contact_with_static;
  Signal<StaticBodyComponent*, math::vec2>  end_contact_with_static;
  Signal<DynamicBodyComponent*, math::vec2> start_contact_with_dynamic;
  Signal<DynamicBodyComponent*, math::vec2> end_contact_with_dynamic;

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

  void on_detach(double time);

  void       apply_global_force(math::vec2 const& val, bool wake_up = true);
  void       apply_local_force(math::vec2 const& val, bool wake_up = true);
  void       apply_torque(float val, bool wake_up = true);
  void       apply_local_linear_impulse(math::vec2 const& val, bool wake_up = true);
  void       apply_global_linear_impulse(math::vec2 const& val, bool wake_up = true);
  void       apply_angular_impulse(float val, bool wake_up = true);

  void       set_linear_velocity(math::vec2 const& val);
  math::vec2 get_linear_velocity();
  float      get_speed();

  void       set_angular_velocity(float val);
  float      get_angular_velocity();

  void       set_transform(math::vec2 const& pos, float rot);

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void init();

  mutable b2Body* body_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_DYNAMIC_BODY_COMPONENT_HPP
