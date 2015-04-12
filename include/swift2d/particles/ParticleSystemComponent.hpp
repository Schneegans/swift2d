////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP
#define SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/components/DepthComponent.hpp>
#include <swift2d/textures/Texture.hpp>

#include <unordered_set>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ParticleSystem;
typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;

class ParticleSystemComponent;
typedef std::shared_ptr<ParticleSystemComponent>       ParticleSystemComponentPtr;
typedef std::shared_ptr<const ParticleSystemComponent> ConstParticleSystemComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL ParticleSystemComponent : public Component,
                                          public DepthComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  enum class CollisionModeEnum {
    NO_COLLISION,
    REFLECT_ON_COLLISION,
    DIE_ON_COLLISION
  };

  struct Serialized : public SerializedComponent {
    float Mass;
    float LinearDamping;
    float AngularDamping;

    float Life;
    float LifeVariance;

    float Velocity;
    float VelocityVariance;

    float AngularVelocity;
    float AngularVelocityVariance;

    float Direction;
    float DirectionVariance;

    float Rotation;
    float RotationVariance;

    float RotateToSpawnDirection;
    float PositionVariance;

    CollisionModeEnum CollisionMode;

    ParticleSystemPtr System;
  };

  // ---------------------------------------------------------------- properties
  Int32 MaxCount;

  Float Mass;
  Float LinearDamping;
  Float AngularDamping;

  Float Life;
  Float LifeVariance;

  Float Velocity;
  Float VelocityVariance;

  Float AngularVelocity;
  Float AngularVelocityVariance;

  Float Direction;
  Float DirectionVariance;

  Float Rotation;
  Float RotationVariance;

  Float RotateToSpawnDirection;
  Float PositionVariance;

  Property<CollisionModeEnum> CollisionMode;

  // ----------------------------------------------------- contruction interface
  ParticleSystemComponent();

  // ------------------------------------------------------------ public methods
  void spawn(math::vec2 const& pos, float rot, unsigned count = 1);
  void spawn(math::vec2 const& pos, float rot, math::vec2 const& vel , unsigned count = 1);
  void spawn(std::vector<std::pair<math::vec3, math::vec2>> const& pos_rot_vel);

  virtual void update(double time);
  virtual void serialize(ParticleSystemComponent::Serialized& serialized) const;
  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ParticleSystemPtr particle_system_;
};

std::ostream& operator<<(std::ostream& os, ParticleSystemComponent::CollisionModeEnum const& obj);
std::istream& operator>>(std::istream& is, ParticleSystemComponent::CollisionModeEnum& obj);

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP
