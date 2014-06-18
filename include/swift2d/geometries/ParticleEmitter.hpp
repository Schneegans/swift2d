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
  Bool            InWorldSpace;
  AnimatedFloat   Life;
  AnimatedFloat   Density;
  TextureProperty Texture;

  // ----------------------------------------------------- contruction interface

  // Creates a new component and returns a shared pointer.
  static ParticleEmitterPtr create() {
    auto result(std::make_shared<ParticleEmitter>());
    result->InWorldSpace = true;
    result->Life         = 10.f;
    result->Density      = 10.f;
    return result;
  }

  // creates a copy from this
  ParticleEmitterPtr create_copy() const {
    return std::make_shared<ParticleEmitter>(*this);
  }

  // ------------------------------------------------------------ public methods
  void update(double time) {
    Life.update(time);
    Density.update(time);
  }


};

}

#endif // SWIFT2D_PARTICLE_EMITTER_HPP
