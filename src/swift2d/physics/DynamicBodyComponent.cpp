////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/DynamicBodyComponent.hpp>

#include <swift2d/physics/Physics.hpp>
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/math/transformations.hpp>

#include <Box2D/Box2D.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DynamicBodyComponent::DynamicBodyComponent()
  : Radius(1.f)
  , body_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

DynamicBodyComponent::~DynamicBodyComponent() {
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::apply_global_force(math::vec2 const& direction) {
  init();
  body_->ApplyForceToCenter(b2Vec2(direction.x(), direction.y()), true);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::apply_local_force(math::vec2 const& direction) {
  init();
  body_->ApplyForceToCenter(body_->GetWorldVector(b2Vec2(direction.x(), direction.y())), true);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::apply_torque(float val) {
  init();
  body_->ApplyTorque(val, true);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::set_linear_velocity(math::vec2 const& val) {
  init();
  body_->SetLinearVelocity(b2Vec2(val.x(), val.y()));
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 DynamicBodyComponent::get_linear_velocity() const {
  init();
  auto v = body_->GetLinearVelocity();
  return math::vec2(v.x, v.y);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::set_angular_velocity(float val) {
  init();
  body_->SetAngularVelocity(val);
}

////////////////////////////////////////////////////////////////////////////////

float DynamicBodyComponent::get_angular_velocity() const {
  init();
  return body_->GetAngularVelocity();
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::update(double time) {
  init();

  b2Vec2 position = body_->GetPosition();
  float angle     = body_->GetAngle();

  auto transform(get_user()->Transform.get());

  math::set_translation(transform, position.x, position.y);
  math::set_rotation(transform, angle);

  get_user()->Transform.set(transform);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::init() const {
  if (!body_) {
    body_ = Physics::instance()->add(this);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
