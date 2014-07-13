////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GRAVITY_SOURCE_COMPONENT_HPP
#define SWIFT2D_GRAVITY_SOURCE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <iostream>

// forward declares ------------------------------------------------------------
class b2Body;

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class GravitySourceComponent;
typedef std::shared_ptr<GravitySourceComponent>       GravitySourceComponentPtr;
typedef std::shared_ptr<const GravitySourceComponent> ConstGravitySourceComponentPtr;

// -----------------------------------------------------------------------------
class GravitySourceComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float Density;

  // ---------------------------------------------------- construction interface
  GravitySourceComponent();
  ~GravitySourceComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static GravitySourceComponentPtr create(Args&& ... a) {
    return std::make_shared<GravitySourceComponent>(a...);
  }

  // creates a copy from this
  GravitySourceComponentPtr create_copy() const {
    return std::make_shared<GravitySourceComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "GravitySourceComponent"; }

  virtual void accept(SavableObjectVisitor& visitor) {
    Component::accept(visitor);
    visitor.add_member("Density", Density);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void init() const;

  mutable bool dirty_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_GRAVITY_SOURCE_COMPONENT_HPP
