////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleSystemComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleSystemComponent::ParticleSystemComponent()
  : Depth(0.f)
  , Mass(0.f)
  , LinearDamping(0.f)
  , AngularDamping(0.f)
  , particle_system_(ParticleSystem::create()) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::add_emitter(ParticleEmitterComponentPtr const& emitter) {
  emitters_.insert(emitter);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::remove_emitter(ParticleEmitterComponentPtr const& emitter) {
  emitters_.erase(emitter);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::update(double time) {
  DrawableComponent::update(time);
  particle_system_->update(emitters_, time);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::update_particles(RenderContext const& ctx) {
  particle_system_->update_particles(
    emitters_, Mass(), LinearDamping(), AngularDamping(), ctx);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::draw_particles(RenderContext const& ctx) {
  particle_system_->draw_particles(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  DrawableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_member("Mass", Mass);
  visitor.add_member("LinearDamping", LinearDamping);
  visitor.add_member("AngularDamping", AngularDamping);
}

////////////////////////////////////////////////////////////////////////////////

}
