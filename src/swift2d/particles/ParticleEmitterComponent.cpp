////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleEmitterComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleEmitterComponent::ParticleEmitterComponent()
  : Density (1.f)
  , particles_to_spawn_(0.f) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleEmitterComponent::update(double time) {

  TransformableComponent::update(time);

  if (ParticleSystem) {
    particles_to_spawn_ += time * Density();
    auto pos(math::get_translation(WorldTransform()));
    auto rot(math::get_rotation(WorldTransform()));

    while (particles_to_spawn_ > 1.f) {
      --particles_to_spawn_;
      ParticleSystem->spawn(math::vec3(pos.x(), pos.y(), rot));
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleEmitterComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Density", Density);
}

////////////////////////////////////////////////////////////////////////////////

}
