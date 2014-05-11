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
  Vec2 pSize;

  // ----------------------------------------------------- contruction interface
  CameraComponent() : pSize(math::vec2(1.f, 1.f)) {}

  // Creates a new component and returns a shared pointer.
  static CameraComponentPtr create() {
    return std::make_shared<CameraComponent>();
  }

  CameraComponentPtr create_copy() const {
    return std::make_shared<CameraComponent>(*this);
  }
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_CAMERA_COMPONENT_HPP
