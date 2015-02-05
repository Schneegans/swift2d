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
  update_world_transform();
}

////////////////////////////////////////////////////////////////////////////////

void TransformableComponent::update_world_transform() {
  if (get_user()) {
    WorldTransform = get_user()->WorldTransform.get() * Transform.get();
  } else {
    WorldTransform = Transform.get();
  }
}

////////////////////////////////////////////////////////////////////////////////

float TransformableComponent::get_rotation() const {
  return math::get_rotation(Transform.get());
}
float TransformableComponent::get_world_rotation() const {
  return math::get_rotation(WorldTransform.get());
}
math::vec2 TransformableComponent::get_direction() const {
  return (Transform.get() * math::vec3(1, 0, 0)).xy();
}
math::vec2 TransformableComponent::get_world_direction() const {
  return (WorldTransform.get() * math::vec3(1, 0, 0)).xy();
}
math::vec2 TransformableComponent::get_position() const {
  return math::get_translation(Transform.get());
}
math::vec2 TransformableComponent::get_world_position() const {
  return math::get_translation(WorldTransform.get());
}

////////////////////////////////////////////////////////////////////////////////

void TransformableComponent::scale(math::vec2 const& scale) {
  math::mat3 t(Transform());
  math::scale(t, scale);
  Transform = t;
}
void TransformableComponent::scale(float scale) {
  math::mat3 t(Transform());
  math::scale(t, scale);
  Transform = t;
}
void TransformableComponent::scale(float x, float y) {
  math::mat3 t(Transform());
  math::scale(t, x, y);
  Transform = t;
}
void TransformableComponent::rotate(float angle) {
  math::mat3 t(Transform());
  math::rotate(t, angle);
  Transform = t;
}
void TransformableComponent::translate(math::vec2 const& delta) {
  math::mat3 t(Transform());
  math::translate(t, delta);
  Transform = t;
}
void TransformableComponent::translate(float x, float y) {
  math::mat3 t(Transform());
  math::translate(t, x, y);
  Transform = t;
}

////////////////////////////////////////////////////////////////////////////////

void TransformableComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_member("Transform", Transform);
}

////////////////////////////////////////////////////////////////////////////////

}
