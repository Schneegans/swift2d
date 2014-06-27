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
#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/properties.hpp>
#include <swift2d/utils/Color.hpp>
#include <swift2d/textures/Texture.hpp>

#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ParticleEmitter;
typedef std::shared_ptr<ParticleEmitter>       ParticleEmitterPtr;
typedef std::shared_ptr<const ParticleEmitter> ConstParticleEmitterPtr;
typedef Property<ParticleEmitterPtr>           ParticleEmitterProperty;

// -----------------------------------------------------------------------------
class ParticleEmitter : public SavableObject {

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

  Float           StartOpacity;
  Float           EndOpacity;

  Float           RotationSpeed;
  Float           RotationSpeedVariance;

  ColorProperty   StartColor;
  ColorProperty   EndColor;

  Float           Density;

  Float           StartScale;
  Float           EndScale;


  // ----------------------------------------------------- contruction interface
  ParticleEmitter();

  // ------------------------------------------------------------ public methods
  virtual bool serialize_as_light() const = 0;

  virtual void draw(RenderContext const& ctx, math::mat3 const& transform, int count) const = 0;

  virtual void spawn(math::mat3 transform, math::vec2& position, float& max_age,
             math::vec2& direction, float& rotation, float& rotation_speed) const = 0;

  virtual void accept(SavableObjectVisitor& visitor) {
    visitor.add_member("WorldSpacePosition", WorldSpacePosition);
    visitor.add_member("WorldSpaceDirection", WorldSpaceDirection);
    visitor.add_member("Life", Life);
    visitor.add_member("LifeVariance", LifeVariance);
    visitor.add_member("Position", Position);
    visitor.add_member("Direction", Direction);
    visitor.add_member("DirectionVariance", DirectionVariance);
    visitor.add_member("StartOpacity", StartOpacity);
    visitor.add_member("EndOpacity", EndOpacity);
    visitor.add_member("RotationSpeed", RotationSpeed);
    visitor.add_member("RotationSpeedVariance", RotationSpeedVariance);
    visitor.add_member("StartColor", StartColor);
    visitor.add_member("EndColor", EndColor);
    visitor.add_member("Density", Density);
    visitor.add_member("StartScale", StartScale);
    visitor.add_member("EndScale", EndScale);
  }
};

}

#endif // SWIFT2D_PARTICLE_EMITTER_HPP
