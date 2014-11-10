////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP
#define SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/particles/ParticleEmitterComponent.hpp>
#include <swift2d/textures/Texture.hpp>

#include <unordered_set>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ParticleSystem;
typedef std::shared_ptr<ParticleSystem>       ParticleSystemPtr;

class ParticleSystemComponent;
typedef std::shared_ptr<ParticleSystemComponent>       ParticleSystemComponentPtr;
typedef std::shared_ptr<const ParticleSystemComponent> ConstParticleSystemComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL ParticleSystemComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface

 public:
  struct Serialized : public SerializedComponent {
    float Mass;

    float Life;
    float LifeVariance;

    float Velocity;
    float VelocityVariance;
    float LinearDamping;

    float AngularVelocity;
    float AngularVelocityVariance;
    float AngularDamping;

    float RotationVariance;
    float PositionVariance;

    std::vector<SerializedEmitter> Emitters;
    ParticleSystemPtr System;
  };

  // ---------------------------------------------------------------- properties
  Int32 MaxCount;
  Float Depth;
  Float Mass;

  Float Life;
  Float LifeVariance;

  Float Velocity;
  Float VelocityVariance;
  Float LinearDamping;

  Float AngularVelocity;
  Float AngularVelocityVariance;
  Float AngularDamping;

  Float RotationVariance;
  Float PositionVariance;

  // ----------------------------------------------------- contruction interface
  ParticleSystemComponent();

  // ------------------------------------------------------------ public methods
  void add_emitter(ParticleEmitterComponent const* emitter);
  void remove_emitter(ParticleEmitterComponent const* emitter);
  void spawn_once(SerializedEmitter const& emitter);

  virtual void serialize(ParticleSystemComponent::Serialized& serialized) const;
  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ParticleSystemPtr particle_system_;

  std::unordered_set<ParticleEmitterComponent const*> emitters_;
  mutable std::vector<SerializedEmitter>              once_emitters_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP
