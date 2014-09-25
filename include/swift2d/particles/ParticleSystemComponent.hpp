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
class SWIFT_DLL ParticleSystemComponent;
typedef std::shared_ptr<ParticleSystemComponent>       ParticleSystemComponentPtr;
typedef std::shared_ptr<const ParticleSystemComponent> ConstParticleSystemComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL ParticleSystemComponent : public DrawableComponent {

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
  void add_emitter(ParticleEmitterComponent const* emitter);
  void remove_emitter(ParticleEmitterComponent const* emitter);

  void spawn_once(SerializedEmitter const& emitter);

  void update_particles(RenderContext const& ctx);
  void draw_particles(RenderContext const& ctx);

  virtual void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ParticleSystemPtr particle_system_;

  std::unordered_set<ParticleEmitterComponent const*> emitters_;
  mutable std::vector<SerializedEmitter>              serialized_emitters_;
  mutable std::vector<SerializedEmitter>              once_emitters_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP
