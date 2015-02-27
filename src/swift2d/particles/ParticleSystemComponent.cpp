////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleSystemComponent.hpp>

#include <swift2d/particles/ParticleSystem.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleSystemComponent::ParticleSystemComponent()
  : MaxCount                (5000)
  , Mass                    (0.f)
  , Life                    (1.f)
  , LifeVariance            (0.f)
  , Velocity                (0.f)
  , VelocityVariance        (0.f)
  , LinearDamping           (0.f)
  , AngularVelocity         (0.f)
  , AngularVelocityVariance (0.f)
  , AngularDamping          (0.f)
  , Direction               (0.0f)
  , DirectionVariance       (0.0f)
  , Rotation                (0.0f)
  , RotationVariance        (0.0f)
  , RotateToSpawnDirection  (1.0f)
  , PositionVariance        (0.0f)
  , CollisionMode           (CollisionModeEnum::NO_COLLISION)
  , particle_system_(ParticleSystem::create(MaxCount())) {

  MaxCount.on_change().connect([&](int val){
    particle_system_->set_max_count(val);
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::update(double time) {
  Component::update(time);
  DepthComponent::update(time, get_user());
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::spawn(math::vec2 const& pos, float rot, unsigned count) {
  particle_system_->spawn(math::vec3(pos.x(), pos.y(), rot), count);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::spawn(math::vec2 const& pos, float rot, math::vec2 const& vel, unsigned count) {
  particle_system_->spawn(math::vec3(pos.x(), pos.y(), rot), vel, count);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::spawn(std::vector<std::pair<math::vec3, math::vec2>> const& pos_rot_vel) {
  particle_system_->spawn(pos_rot_vel);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::serialize(ParticleSystemComponent::Serialized& serialized) const {

  serialized.Depth = WorldDepth();
  serialized.Mass = Mass();

  serialized.Life = Life();
  serialized.LifeVariance = LifeVariance();

  serialized.Velocity = Velocity();
  serialized.VelocityVariance = VelocityVariance();
  serialized.LinearDamping = LinearDamping();

  serialized.AngularVelocity = AngularVelocity();
  serialized.AngularVelocityVariance = AngularVelocityVariance();
  serialized.AngularDamping = AngularDamping();

  serialized.Direction = Direction();
  serialized.DirectionVariance = DirectionVariance();

  serialized.Rotation = Rotation();
  serialized.RotationVariance = RotationVariance();

  serialized.RotateToSpawnDirection = RotateToSpawnDirection();
  serialized.PositionVariance = PositionVariance();

  serialized.CollisionMode = CollisionMode();

  serialized.System = particle_system_;
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  DepthComponent::accept(visitor);
  visitor.add_member("MaxCount",                  MaxCount);
  visitor.add_member("Mass",                      Mass);
  visitor.add_member("LinearDamping",             LinearDamping);
  visitor.add_member("AngularDamping",            AngularDamping);
  visitor.add_member("Life",                      Life);
  visitor.add_member("LifeVariance",              LifeVariance);
  visitor.add_member("Direction",                 Direction);
  visitor.add_member("DirectionVariance",         DirectionVariance);
  visitor.add_member("Rotation",                  Rotation);
  visitor.add_member("RotationVariance",          RotationVariance);
  visitor.add_member("Velocity",                  Velocity);
  visitor.add_member("VelocityVariance",          VelocityVariance);
  visitor.add_member("AngularVelocity",           AngularVelocity);
  visitor.add_member("AngularVelocityVariance",   AngularVelocityVariance);
  visitor.add_member("RotateToSpawnDirection",    RotateToSpawnDirection);
  visitor.add_member("PositionVariance",          PositionVariance);
  visitor.add_member("CollisionMode",             CollisionMode);
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, ParticleSystemComponent::CollisionModeEnum const& obj) {
  os << static_cast<int>(obj);
  return os;
}

////////////////////////////////////////////////////////////////////////////////

std::istream& operator>>(std::istream& is, ParticleSystemComponent::CollisionModeEnum& obj) {
  int tmp(0);
  is >> tmp;
  obj = static_cast<ParticleSystemComponent::CollisionModeEnum>(tmp);
  return is;
}

////////////////////////////////////////////////////////////////////////////////

}
