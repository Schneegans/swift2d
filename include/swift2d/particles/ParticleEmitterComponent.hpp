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
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ParticleEmitterComponent;
typedef std::shared_ptr<ParticleEmitterComponent>       ParticleEmitterComponentPtr;
typedef std::shared_ptr<const ParticleEmitterComponent> ConstParticleEmitterComponentPtr;
typedef Property<ParticleEmitterComponentPtr>           ParticleEmitterComponentProperty;

struct SWIFT_DLL SerializedEmitter {
  float Density;
  math::vec3 PosRot;
  ParticleEmitterComponent const* Self;
};

// -----------------------------------------------------------------------------
class SWIFT_DLL ParticleEmitterComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float Density;

  // ----------------------------------------------------- contruction interface
  ParticleEmitterComponent();

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "ParticleEmitterComponent"; }

  virtual void accept(SavableObjectVisitor& visitor);

  SerializedEmitter make_serialized_emitter() const;
};

}

#endif // SWIFT2D_PARTICLE_EMITTER_HPP
