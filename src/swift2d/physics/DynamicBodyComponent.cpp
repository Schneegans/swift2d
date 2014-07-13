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
  , Density(1.f)
  , Friction(0.5f)
  , Restitution(0.5f)
  , LinearDamping(0.5f)
  , AngularDamping(0.5f)
  , body_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

DynamicBodyComponent::~DynamicBodyComponent() {
  if (body_) {
    Physics::instance()->remove(body_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::apply_global_force(math::vec2 const& val) {
  init();
  body_->ApplyForceToCenter(b2Vec2(val.x(), val.y()), true);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::apply_local_force(math::vec2 const& val) {
  init();
  body_->ApplyForceToCenter(body_->GetWorldVector(b2Vec2(val.x(), val.y())), true);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::apply_torque(float val) {
  init();
  body_->ApplyTorque(val, true);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::apply_local_linear_impulse(math::vec2 const& val) {
  init();
  body_->ApplyLinearImpulse(body_->GetWorldVector(b2Vec2(val.x(), val.y())), body_->GetWorldCenter(), true);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::apply_global_linear_impulse(math::vec2 const& val) {
  init();
  body_->ApplyLinearImpulse(b2Vec2(val.x(), val.y()), body_->GetWorldCenter(), true);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::apply_angular_impulse(float val) {
  init();
  body_->ApplyAngularImpulse(val, true);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::set_linear_velocity(math::vec2 const& val) {
  init();
  body_->SetLinearVelocity(b2Vec2(val.x(), val.y()));
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 DynamicBodyComponent::get_linear_velocity() {
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

float DynamicBodyComponent::get_angular_velocity() {
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

void DynamicBodyComponent::init() {
  if (!body_) {
    body_ = Physics::instance()->add(this);

    Radius.on_change().connect([&](float val){
      body_->GetFixtureList()->GetShape()->m_radius = val;
      body_->ResetMassData();
    });
    Density.on_change().connect([&](float val){
      body_->GetFixtureList()->SetDensity(val);
      body_->ResetMassData();
    });
    Friction.on_change().connect([&](float val){
      body_->GetFixtureList()->SetFriction(val);
    });
    Restitution.on_change().connect([&](float val){
      body_->GetFixtureList()->SetRestitution(val);
    });
    LinearDamping.on_change().connect([&](float val){
      body_->SetLinearDamping(val);
    });
    AngularDamping.on_change().connect([&](float val){
      body_->SetAngularDamping(val);
    });
  }
}

////////////////////////////////////////////////////////////////////////////////

}
