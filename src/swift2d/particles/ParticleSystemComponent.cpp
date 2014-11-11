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
  , LinearDamping           (0.f)
  , AngularDamping          (0.f)
  , Life                    (10.f)
  , LifeVariance            (3.f)
  , RotationVariance        (0.3f)
  , Velocity                (0.3f)
  , VelocityVariance        (0.3f)
  , AngularVelocity         (0.f)
  , AngularVelocityVariance (0.f)
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

void ParticleSystemComponent::add_emitter(ParticleEmitterComponent const* emitter) {
  emitters_.insert(emitter);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::remove_emitter(ParticleEmitterComponent const* emitter) {
  emitters_.erase(emitter);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::spawn_once(SerializedEmitter const& emitter) {
  once_emitters_.push_back(emitter);
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

  serialized.RotationVariance = RotationVariance();
  serialized.PositionVariance = PositionVariance();

  serialized.System = particle_system_;
  serialized.Emitters.resize(emitters_.size() + once_emitters_.size());

  int i(0);
  for (auto const& emitter: emitters_) {
    serialized.Emitters[i++] = emitter->make_serialized_emitter();
  }
  for (auto const& emitter: once_emitters_) {
    serialized.Emitters[i++] = emitter;
  }
  once_emitters_.clear();
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
  visitor.add_member("RotationVariance",          RotationVariance);
  visitor.add_member("Velocity",                  Velocity);
  visitor.add_member("VelocityVariance",          VelocityVariance);
  visitor.add_member("AngularVelocity",           AngularVelocity);
  visitor.add_member("AngularVelocityVariance",   AngularVelocityVariance);
  visitor.add_member("PositionVariance",          PositionVariance);
}

////////////////////////////////////////////////////////////////////////////////

}
