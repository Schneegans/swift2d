////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleSystemComponent.hpp>

#include <swift2d/particles/ParticleSystem.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleSystemComponent::ParticleSystemComponent()
  : MaxCount                (1000)
  , Mass                    (0.f)
  , Life                    (10.f)
  , LifeVariance            (3.f)
  , Velocity                (0.3f)
  , VelocityVariance        (0.3f)
  , LinearDamping           (0.f)
  , AngularVelocity         (0.f)
  , AngularVelocityVariance (0.f)
  , AngularDamping          (0.f)
  , Rotation                (0.0f)
  , RotationVariance        (0.3f)
  , PositionVariance        (0.0f)
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

void ParticleSystemComponent::spawn(math::vec3 const& pos_rot, unsigned count) {
  particle_system_->spawn(pos_rot, count);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::spawn(std::vector<math::vec3> const& pos_rots) {
  particle_system_->spawn(pos_rots);
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

  serialized.Rotation = Rotation();
  serialized.RotationVariance = RotationVariance();
  serialized.PositionVariance = PositionVariance();

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
  visitor.add_member("Rotation",                  Rotation);
  visitor.add_member("RotationVariance",          RotationVariance);
  visitor.add_member("Velocity",                  Velocity);
  visitor.add_member("VelocityVariance",          VelocityVariance);
  visitor.add_member("AngularVelocity",           AngularVelocity);
  visitor.add_member("AngularVelocityVariance",   AngularVelocityVariance);
  visitor.add_member("PositionVariance",          PositionVariance);
}

////////////////////////////////////////////////////////////////////////////////

}
