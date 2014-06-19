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

}
