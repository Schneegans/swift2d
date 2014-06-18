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
#include <swift2d/geometries/CPUParticleSystem.hpp>
#include <swift2d/geometries/ParticleEmitter.hpp>

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
  Float                   Depth;
  ParticleEmitterProperty Emitter;

  // ----------------------------------------------------- contruction interface
  ParticleSystemComponent()
    : Depth(0.f)
    , particle_system_(CPUParticleSystem::create(this)) {}

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static ParticleSystemComponentPtr create(Args&& ... a) {
    return std::make_shared<ParticleSystemComponent>(a...);
  }

  // creates a copy from this
  ParticleSystemComponentPtr create_copy() const {
    return std::make_shared<ParticleSystemComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  void update(double time) {
    DrawableComponent::update(time);
    Emitter()->update(time);
    particle_system_->update(time);
  }

  void draw(RenderContext const& ctx) {
    particle_system_->draw(ctx);
  }

  void serialize(SerializedScenePtr& scene) const {
    scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  CPUParticleSystemPtr particle_system_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP
