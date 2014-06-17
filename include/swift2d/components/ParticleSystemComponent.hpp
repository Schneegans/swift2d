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
#include <swift2d/geometries/ParticleSystem.hpp>

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
  Float            Depth;
  MaterialProperty Material;

  // ----------------------------------------------------- contruction interface
  ParticleSystemComponent() : Depth(0.f) {}

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
    ParticleSystem::instance()->update(time);
  }

  void draw(RenderContext const& ctx) {
    ParticleSystem::instance()->draw(ctx, WorldTransform());
  }

  void serialize(SerializedScenePtr& scene) const {
    scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
  }

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP
