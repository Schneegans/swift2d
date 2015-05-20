////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/PhysicsBodyComponent.hpp>

#include <swift2d/physics/Physics.hpp>
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/math/transformations.hpp>

#include <Box2D/Box2D.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PhysicsBodyComponent::PhysicsBodyComponent()
  : BodyType(BodyTypeEnum::DYNAMIC)
  , Mass(1.f)
  , Friction(0.5f)
  , Restitution(0.5f)
  , LinearDamping(0.5f)
  , AngularDamping(0.5f)
  , GravityScale(1.f)
  , ShockWaveScale(1.f)
  , IsBullet(false)
  , FixedRotation(false)
  , IsSensor(false)
  , Sleep(false)
  , Group(0)
  , Mask(-1)
  , Category(0)
  , body_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

PhysicsBodyComponent::~PhysicsBodyComponent() {
  if (body_) {
    Physics::get().remove(body_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::on_detach(double time) {
  update(time);
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::on_init() {
  if (!body_) {
    body_ = Physics::get().add(this);

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
    BodyType.on_change().connect([&](BodyTypeEnum val){
      body_->SetType(static_cast<b2BodyType>(val));
      return true;
    });
    Mass.on_change().connect([&](float val){
      auto transform(get_user()->WorldTransform());
      math::vec2 scale(math::get_scale(transform));
      body_->GetFixtureList()->SetDensity(val / scale.x() / scale.y());
      body_->ResetMassData();
      return true;
    });
    Friction.on_change().connect([&](float val){
      body_->GetFixtureList()->SetFriction(val);
      return true;
    });
    IsBullet.on_change().connect([&](bool val){
      body_->SetBullet(val);
      return true;
    });
    FixedRotation.on_change().connect([&](bool val){
      body_->SetFixedRotation(val);
      return true;
    });
    IsSensor.on_change().connect([&](bool val){
      if (body_) {
        LOG_WARNING << "IsSensor cannot be set on initialized bodies!" << std::endl;
      }
      return true;
    });
    Sleep.on_change().connect([&](bool val){
      body_->SetAwake(!val);
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

void PhysicsBodyComponent::apply_global_force(math::vec2 const& val, bool wake_up) {
  if (body_) {
    body_->ApplyForceToCenter(b2Vec2(val.x(), val.y()), wake_up);
  }
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::apply_local_force(math::vec2 const& val, bool wake_up) {
  if (body_) {
    body_->ApplyForceToCenter(body_->GetWorldVector(b2Vec2(val.x(), val.y())), wake_up);
  }
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::apply_torque(float val, bool wake_up) {
  if (body_) {
    body_->ApplyTorque(val, wake_up);
  }
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::apply_local_linear_impulse(math::vec2 const& val, bool wake_up) {
  if (body_) {
    body_->ApplyLinearImpulse(body_->GetWorldVector(b2Vec2(val.x(), val.y())), body_->GetWorldCenter(), wake_up);
  }
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::apply_global_linear_impulse(math::vec2 const& val, bool wake_up) {
  if (body_) {
    body_->ApplyLinearImpulse(b2Vec2(val.x(), val.y()), body_->GetWorldCenter(), wake_up);
  }
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::apply_angular_impulse(float val, bool wake_up) {
  if (body_) {
    body_->ApplyAngularImpulse(val, wake_up);
  }
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::set_linear_velocity(math::vec2 const& val) {
  if (body_) {
    body_->SetLinearVelocity(b2Vec2(val.x(), val.y()));
  }
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 PhysicsBodyComponent::get_linear_velocity() {
  if (body_) {
    auto v = body_->GetLinearVelocity();
    return math::vec2(v.x, v.y);
  }
  return math::vec2();
}

////////////////////////////////////////////////////////////////////////////////

float PhysicsBodyComponent::get_speed() {
  if (body_) {
    auto v = body_->GetLinearVelocity();
    return math::vec2(v.x, v.y).Length();
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::set_angular_velocity(float val) {
  if (body_) {
    body_->SetAngularVelocity(val);
  }
}

////////////////////////////////////////////////////////////////////////////////

float PhysicsBodyComponent::get_angular_velocity() {
  if (body_) {
    return body_->GetAngularVelocity();
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::set_transform(math::vec2 const& pos, float rot) {
  if (body_) {
    body_->SetTransform(b2Vec2(pos.x(), pos.y()), rot);

    auto transform(get_user()->Transform.get());
    math::set_rotation(transform, rot);
    math::set_translation(transform, pos.x(), pos.y());
    get_user()->Transform.set(transform);
    get_user()->update_world_transform();
  }
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::update(double time) {

  if (BodyType() != BodyTypeEnum::STATIC) {
    b2Vec2 position = body_->GetPosition();
    float angle     = body_->GetAngle();

    auto transform(get_user()->Transform.get());
    math::set_rotation(transform, angle);
    math::set_translation(transform, position.x, position.y);
    get_user()->Transform.set(transform);
    get_user()->update_world_transform();
  }

  Sleep.set_with_no_emit(!body_->IsAwake());
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsBodyComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_object_property("Shape", Shape);
  visitor.add_member("BodyType", BodyType);
  visitor.add_member("Mass", Mass);
  visitor.add_member("Friction", Friction);
  visitor.add_member("Restitution", Restitution);
  visitor.add_member("LinearDamping", LinearDamping);
  visitor.add_member("AngularDamping", AngularDamping);
  visitor.add_member("GravityScale", GravityScale);
  visitor.add_member("ShockWaveScale", ShockWaveScale);
  visitor.add_member("Group", Group);
  visitor.add_member("IsBullet", IsBullet);
  visitor.add_member("FixedRotation", FixedRotation);
  visitor.add_member("IsSensor", IsSensor);
  visitor.add_member("Sleep", Sleep);
  visitor.add_member("Mask", Mask);
  visitor.add_member("Category", Category);
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, PhysicsBodyComponent::BodyTypeEnum const& obj) {
  os << static_cast<int>(obj);
  return os;
}

////////////////////////////////////////////////////////////////////////////////

std::istream& operator>>(std::istream& is, PhysicsBodyComponent::BodyTypeEnum& obj) {
  int tmp(0);
  is >> tmp;
  obj = static_cast<PhysicsBodyComponent::BodyTypeEnum>(tmp);
  return is;
}

////////////////////////////////////////////////////////////////////////////////

}
