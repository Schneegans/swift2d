////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PHYSICS_BODY_COMPONENT_HPP
#define SWIFT2D_PHYSICS_BODY_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/physics/CollisionShape.hpp>

// forward declares ------------------------------------------------------------
class b2Body;

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class PhysicsBodyComponent;
typedef std::shared_ptr<PhysicsBodyComponent>       PhysicsBodyComponentPtr;
typedef std::shared_ptr<const PhysicsBodyComponent> ConstPhysicsBodyComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL PhysicsBodyComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  enum class BodyTypeEnum {
    STATIC = 0,
    KINEMATIC,
    DYNAMIC
  };

  // ---------------------------------------------------------------- properties
  Property<BodyTypeEnum>  BodyType;
  CollisionShapeProperty  Shape;
  Float                   Mass;
  Float                   Friction;
  Float                   Restitution;
  Bool                    FixedRotation;
  Bool                    IsSensor;

  Int16                   Group;
  UInt16                  Mask;
  UInt16                  Category;

  // dynamic body properties
  Float                   LinearDamping;
  Float                   AngularDamping;
  Float                   GravityScale;
  Bool                    IsBullet;
  Bool                    Sleep;


  // ------------------------------------------------------------------- signals
  Signal<PhysicsBodyComponent*, PhysicsBodyComponent*, math::vec2> start_contact;
  Signal<PhysicsBodyComponent*, PhysicsBodyComponent*, math::vec2> end_contact;

  Signal<PhysicsBodyComponent*, PhysicsBodyComponent*, bool&> pre_solve;

  // ---------------------------------------------------- construction interface
  PhysicsBodyComponent();
  ~PhysicsBodyComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static PhysicsBodyComponentPtr create(Args&& ... a) {
    return std::make_shared<PhysicsBodyComponent>(a...);
  }

  // creates a copy from this
  PhysicsBodyComponentPtr create_copy() const {
    return std::make_shared<PhysicsBodyComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "PhysicsBodyComponent"; }

  virtual void on_detach(double time);
  virtual void on_init();
  virtual void update(double time);
  virtual void accept(SavableObjectVisitor& visitor);

  // call these methods only after on_init has been called!
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

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  mutable b2Body* body_;
};

// -----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, PhysicsBodyComponent::BodyTypeEnum const& obj);
std::istream& operator>>(std::istream& is, PhysicsBodyComponent::BodyTypeEnum& obj);

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_PHYSICS_BODY_COMPONENT_HPP
