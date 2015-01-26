////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PARTICLE_ONCE_EMITTER_HPP
#define SWIFT2D_PARTICLE_ONCE_EMITTER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/particles/ParticleSystemComponent.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ParticleOnceEmitterComponent;
typedef std::shared_ptr<ParticleOnceEmitterComponent>       ParticleOnceEmitterComponentPtr;
typedef std::shared_ptr<const ParticleOnceEmitterComponent> ConstParticleOnceEmitterComponentPtr;
typedef Property<ParticleOnceEmitterComponentPtr>           ParticleOnceEmitterComponentProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL ParticleOnceEmitterComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  ParticleSystemComponentPtr ParticleSystem;
  String                     ParticleSystemLabel;
  Double                     Delay;
  Float                      Amount;
  Bool                       DetachOnEmmission;

  // ----------------------------------------------------- contruction interface
  ParticleOnceEmitterComponent();

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "ParticleOnceEmitterComponent"; }

  virtual void update(double time);
  virtual void accept(SavableObjectVisitor& visitor);
};

}

#endif // SWIFT2D_PARTICLE_ONCE_EMITTER_HPP
