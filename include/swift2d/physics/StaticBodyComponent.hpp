////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_STATIC_BODY_COMPONENT_HPP
#define SWIFT2D_STATIC_BODY_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/physics/CollisionShape.hpp>

// forward declares ------------------------------------------------------------
class b2Body;

namespace swift {

class DynamicBodyComponent;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class StaticBodyComponent;
typedef std::shared_ptr<StaticBodyComponent>       StaticBodyComponentPtr;
typedef std::shared_ptr<const StaticBodyComponent> ConstStaticBodyComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL StaticBodyComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  CollisionShapeProperty  Shape;
  Float                   Friction;
  Float                   Restitution;

  Int16                   Group;
  UInt16                  Mask;
  UInt16                  Category;

  // ------------------------------------------------------------------- signals
  Signal<DynamicBodyComponent*, math::vec2> start_contact_with_dynamic;
  Signal<DynamicBodyComponent*, math::vec2> end_contact_with_dynamic;

  // ---------------------------------------------------- construction interface
  StaticBodyComponent();
  ~StaticBodyComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static StaticBodyComponentPtr create(Args&& ... a) {
    return std::make_shared<StaticBodyComponent>(a...);
  }

  // creates a copy from this
  StaticBodyComponentPtr create_copy() const {
    return std::make_shared<StaticBodyComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "StaticBodyComponent"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  b2Body* body_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_STATIC_BODY_COMPONENT_HPP
