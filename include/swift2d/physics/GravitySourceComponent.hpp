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

#ifndef SWIFT2D_GRAVITY_SOURCE_COMPONENT_HPP
#define SWIFT2D_GRAVITY_SOURCE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>

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
class SWIFT_DLL GravitySourceComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float Mass;
  Float CollisionRadius;

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

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "GravitySourceComponent"; }

  virtual void serialize(SerializedScenePtr& scene) const;

  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_GRAVITY_SOURCE_COMPONENT_HPP
