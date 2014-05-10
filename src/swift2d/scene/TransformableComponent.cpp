////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/TransformableComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void TransformableComponent::update() {
  if (pUser.get()) {
    pWorldTransform = pTransform.get() * pUser.get()->pWorldTransform.get();
  } else {
    pWorldTransform = pTransform.get();
  }
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 TransformableComponent::get_position() const {
  return math::get_position(pTransform.get());
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 TransformableComponent::get_world_position() const {
  return math::get_position(pWorldTransform.get());
}

////////////////////////////////////////////////////////////////////////////////

}