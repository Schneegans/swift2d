////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MOVE_COMPONENT_HPP
#define SWIFT2D_MOVE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class MoveComponent;
typedef std::shared_ptr<MoveComponent>       MoveComponentPtr;
typedef std::shared_ptr<const MoveComponent> ConstMoveComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL MoveComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Vec2 LinearSpeed;
  Float LinearDamping;

  Float AngularSpeed;
  Float AngularDamping;

  Bool IgnoreParentRotation;

  // ----------------------------------------------------- constrution interface
  MoveComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static MoveComponentPtr create(Args&& ... a) {
    return std::make_shared<MoveComponent>(a...);
  }

  // creates a copy from this
  MoveComponentPtr create_copy() const {
    return std::make_shared<MoveComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "MoveComponent"; }

  virtual void update(double time);
  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  float rotation_;
};

}

#endif  // SWIFT2D_MOVE_COMPONENT_HPP
