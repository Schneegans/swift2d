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
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/particles/ParticleSystem.hpp>
#include <swift2d/textures/Texture.hpp>

#include <unordered_set>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ParticleSystemComponent;
typedef std::shared_ptr<ParticleSystemComponent>       ParticleSystemComponentPtr;
typedef std::shared_ptr<const ParticleSystemComponent> ConstParticleSystemComponentPtr;

// -----------------------------------------------------------------------------
class ParticleSystemComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Int   MaxCount;
  Float Depth;
  Float Mass;
  Float LinearDamping;
  Float AngularDamping;

  // ----------------------------------------------------- contruction interface
  ParticleSystemComponent();

  // ------------------------------------------------------------ public methods
  void add_emitter(ParticleEmitterComponentPtr const& emitter);
  void remove_emitter(ParticleEmitterComponentPtr const& emitter);

  void update_particles(RenderContext const& ctx);
  void draw_particles(RenderContext const& ctx);

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ParticleSystemPtr particle_system_;

  std::unordered_set<ParticleEmitterComponentPtr> emitters_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP