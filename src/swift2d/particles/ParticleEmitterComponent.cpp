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
  : WorldSpacePosition    (true)
  , WorldSpaceDirection   (false)
  , Life                  (10.f)
  , LifeVariance          (3.f)
  , Direction             (math::vec2(1.f, 0.f))
  , DirectionVariance     (0.3f)
  , Density               (5.f) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleEmitterComponent::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("WorldSpacePosition", WorldSpacePosition);
  visitor.add_member("WorldSpaceDirection", WorldSpaceDirection);
  visitor.add_member("Life", Life);
  visitor.add_member("LifeVariance", LifeVariance);
  visitor.add_member("Direction", Direction);
  visitor.add_member("DirectionVariance", DirectionVariance);
  visitor.add_member("Density", Density);
}

////////////////////////////////////////////////////////////////////////////////

}
