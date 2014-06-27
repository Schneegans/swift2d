////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/components/TransformableComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void TransformableComponent::update(double time) {
  if (get_user()) {
    WorldTransform = get_user()->WorldTransform.get() * Transform.get();
  } else {
    WorldTransform = Transform.get();
  }
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 TransformableComponent::get_position() const {
  return math::get_translate(Transform.get());
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 TransformableComponent::get_world_position() const {
  return math::get_translate(WorldTransform.get());
}

////////////////////////////////////////////////////////////////////////////////

void TransformableComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_member("Transform", Transform);
}

////////////////////////////////////////////////////////////////////////////////

}
