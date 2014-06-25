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
#include <swift2d/geometries/TextureParticleEmitter.hpp>
#include <swift2d/geometries/LightParticleEmitter.hpp>

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
    , particle_system_(ParticleSystem::create(this)) {}

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
    particle_system_->update(time);
  }

  void draw(RenderContext const& ctx) {
    particle_system_->draw(ctx);
  }

  int get_particle_count() const {
    return particle_system_->get_particle_count();
  }

  void serialize(SerializedScenePtr& scene) const {
    if (Emitter()->serialize_as_light()) {
      scene->lights.insert(std::make_pair(Depth.get(), create_copy()));
    } else {
      scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
    }
  }

  virtual boost::property_tree::ptree to_json() const {
    boost::property_tree::ptree tree;
    tree.put("Type", "ParticleSystemComponent");
    tree.put("Transform", "dummy");
    tree.put("Emitter", "dummy");
    tree.put("Depth", Depth());
    return tree;
  };

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ParticleSystemPtr particle_system_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP
