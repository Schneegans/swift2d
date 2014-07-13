////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_CAMERA_COMPONENT_HPP
#define SWIFT2D_CAMERA_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class CameraComponent;
typedef std::shared_ptr<CameraComponent>       CameraComponentPtr;
typedef std::shared_ptr<const CameraComponent> ConstCameraComponentPtr;

// -----------------------------------------------------------------------------
class CameraComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Vec2 Size;
  Float Parallax;

  // ----------------------------------------------------- contruction interface
  CameraComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static CameraComponentPtr create(Args&& ... a) {
    return std::make_shared<CameraComponent>(a...);
  }

  // creates a copy from this
  CameraComponentPtr create_copy() const {
    return std::make_shared<CameraComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "CameraComponent"; }

  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_CAMERA_COMPONENT_HPP
