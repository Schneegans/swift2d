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
  : Density(1.f)
  , Friction(0.5f)
  , Restitution(0.5f)
  , LinearDamping(0.5f)
  , AngularDamping(0.5f)
  , Group(0)
  , Mask(-1)
  , Category(0)
  , body_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

DynamicBodyComponent::~DynamicBodyComponent() {
  if (body_) {
    Physics::get().remove(body_);
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

float DynamicBodyComponent::get_speed() {
  init();
  auto v = body_->GetLinearVelocity();
  return math::vec2(v.x, v.y).Length();
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

void DynamicBodyComponent::set_transform(math::vec2 const& pos, float rot) {
  init();
  body_->SetTransform(b2Vec2(pos.x(), pos.y()), rot);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::update(double time) {

  init();

  b2Vec2 position = body_->GetPosition();
  float angle     = body_->GetAngle();

  auto transform(get_user()->Transform.get());

  math::set_rotation(transform, angle);
  math::set_translation(transform, position.x, position.y);

  get_user()->Transform.set(transform);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_object_property("Shape", Shape);
  visitor.add_member("Density", Density);
  visitor.add_member("Friction", Friction);
  visitor.add_member("Restitution", Restitution);
  visitor.add_member("LinearDamping", LinearDamping);
  visitor.add_member("AngularDamping", AngularDamping);
  visitor.add_member("Group", Group);
  visitor.add_member("Mask", Mask);
  visitor.add_member("Category", Category);
}

////////////////////////////////////////////////////////////////////////////////

void DynamicBodyComponent::init() {
  if (!body_) {
    body_ = Physics::get().add(this);

    body_->SetActive(Enabled());
    Enabled.on_change().connect([this](bool enabled) {
      if (body_) {
        body_->SetActive(enabled);
      }
      return true;
    });

    Shape.on_change().connect([&](CollisionShapePtr const&){
      LOG_WARNING << "Updating collision shapes is not implmented yet!"
                          << std::endl;
      return true;
    });
    Density.on_change().connect([&](float val){
      body_->GetFixtureList()->SetDensity(val);
      body_->ResetMassData();
      return true;
    });
    Friction.on_change().connect([&](float val){
      body_->GetFixtureList()->SetFriction(val);
      return true;
    });
    Restitution.on_change().connect([&](float val){
      body_->GetFixtureList()->SetRestitution(val);
      return true;
    });
    LinearDamping.on_change().connect([&](float val){
      body_->SetLinearDamping(val);
      return true;
    });
    AngularDamping.on_change().connect([&](float val){
      body_->SetAngularDamping(val);
      return true;
    });
    Group.on_change().connect([&](math::int16 val){
      auto f(body_->GetFixtureList()->GetFilterData());
      f.groupIndex = val;
      body_->GetFixtureList()->SetFilterData(f);
      return true;
    });
    Mask.on_change().connect([&](math::uint16 val){
      auto f(body_->GetFixtureList()->GetFilterData());
      f.maskBits = val;
      body_->GetFixtureList()->SetFilterData(f);
      return true;
    });
    Category.on_change().connect([&](math::uint16 val){
      auto f(body_->GetFixtureList()->GetFilterData());
      f.categoryBits = val;
      body_->GetFixtureList()->SetFilterData(f);
      return true;
    });
  }
}

////////////////////////////////////////////////////////////////////////////////

}
