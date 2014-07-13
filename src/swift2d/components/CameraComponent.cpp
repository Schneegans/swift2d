////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/CameraComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

CameraComponent::CameraComponent()
  : Size(math::vec2(1.f, 1.f))
  , Parallax(1.0) {}

////////////////////////////////////////////////////////////////////////////////

void CameraComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Size", Size);
  visitor.add_member("Parallax", Parallax);
}

////////////////////////////////////////////////////////////////////////////////

}
