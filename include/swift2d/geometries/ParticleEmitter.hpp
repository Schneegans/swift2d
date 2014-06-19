////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PARTICLE_EMITTER_HPP
#define SWIFT2D_PARTICLE_EMITTER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/properties.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>

#include <stack>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// shared pointer type definition ----------------------------------------------
class ParticleEmitter;
typedef std::shared_ptr<ParticleEmitter>       ParticleEmitterPtr;
typedef std::shared_ptr<const ParticleEmitter> ConstParticleEmitterPtr;
typedef Property<ParticleEmitterPtr>           ParticleEmitterProperty;

// -----------------------------------------------------------------------------
class ParticleEmitter {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Bool            WorldSpacePosition;
  Bool            WorldSpaceDirection;

  Float           Life;
  Float           LifeVariance;

  Vec2            Position;
  Vec2            Direction;
  Float           DirectionVariance;

  Float           StartRotation;
  Float           EndRotation;
  Float           RotationVariance;

  Float           StartOpacity;
  Float           EndOpacity;

  Bool            Colorize;
  Color           StartColor;
  Color           EndColor;

  Float           Density;

  Float           StartScale;
  Float           EndScale;

  TextureProperty Texture;

  // ----------------------------------------------------- contruction interface

  // Creates a new component and returns a shared pointer.
  static ParticleEmitterPtr create() {
    auto result(std::make_shared<ParticleEmitter>());
    result->WorldSpacePosition    = true;
    result->WorldSpaceDirection   = false;
    result->Life                  = 10.f;
    result->LifeVariance          = 3.f;
    result->Position              = math::vec2(0.f, 0.f);
    result->Direction             = math::vec2(1.f, 0.f);
    result->DirectionVariance     = 0.3f;
    result->StartRotation         = 5.f;
    result->EndRotation           = 0.f;
    result->RotationVariance      = 1.f;
    result->StartOpacity          = 1.f;
    result->EndOpacity            = 0.f;
    result->Colorize              = false;
    result->StartColor            = Color(1, 1, 1);
    result->EndColor              = Color(1, 1, 1);
    result->Density               = 5.f;
    result->StartScale            = 1.f;
    result->EndScale              = 10.f;
    return result;
  }

  // creates a copy from this
  ParticleEmitterPtr create_copy() const {
    return std::make_shared<ParticleEmitter>(*this);
  }
};

}

#endif // SWIFT2D_PARTICLE_EMITTER_HPP
