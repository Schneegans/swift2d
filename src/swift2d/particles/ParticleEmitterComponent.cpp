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
  : Life                  (10.f)
  , LifeVariance          (3.f)
  , Direction             (0.f)
  , DirectionVariance     (0.3f)
  , Velocity              (0.3f)
  , VelocityVariance      (0.3f)
  , Density               (5.f) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleEmitterComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Life", Life);
  visitor.add_member("LifeVariance", LifeVariance);
  visitor.add_member("Direction", Direction);
  visitor.add_member("DirectionVariance", DirectionVariance);
  visitor.add_member("Velocity", Velocity);
  visitor.add_member("VelocityVariance", VelocityVariance);
  visitor.add_member("Density", Density);
}

////////////////////////////////////////////////////////////////////////////////

}
