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

  // ----------------------------------------------------- contruction interface
  CameraComponent() : Size(math::vec2(1.f, 1.f)) {}

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static CameraComponentPtr create(Args&& ... a) {
    return std::make_shared<CameraComponent>(a...);
  }

  // creates a copy from this
  CameraComponentPtr create_copy() const {
    return std::make_shared<CameraComponent>(*this);
  }

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "CameraComponent"; }
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_CAMERA_COMPONENT_HPP
