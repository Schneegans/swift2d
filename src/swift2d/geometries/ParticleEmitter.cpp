////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/ParticleEmitter.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleEmitter::ParticleEmitter()
  : WorldSpacePosition    (true)
  , WorldSpaceDirection   (false)
  , Life                  (10.f)
  , LifeVariance          (3.f)
  , Position              (math::vec2(0.f, 0.f))
  , Direction             (math::vec2(1.f, 0.f))
  , DirectionVariance     (0.3f)
  , StartOpacity          (1.f)
  , EndOpacity            (0.f)
  , StartColor            (Color(1, 1, 1))
  , EndColor              (Color(1, 1, 1))
  , Density               (5.f)
  , StartScale            (1.f)
  , EndScale              (10.f) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleEmitter::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("WorldSpacePosition", WorldSpacePosition);
  visitor.add_member("WorldSpaceDirection", WorldSpaceDirection);
  visitor.add_member("Life", Life);
  visitor.add_member("LifeVariance", LifeVariance);
  visitor.add_member("Position", Position);
  visitor.add_member("Direction", Direction);
  visitor.add_member("DirectionVariance", DirectionVariance);
  visitor.add_member("StartOpacity", StartOpacity);
  visitor.add_member("EndOpacity", EndOpacity);
  visitor.add_member("StartColor", StartColor);
  visitor.add_member("EndColor", EndColor);
  visitor.add_member("Density", Density);
  visitor.add_member("StartScale", StartScale);
  visitor.add_member("EndScale", EndScale);
}

////////////////////////////////////////////////////////////////////////////////

}
