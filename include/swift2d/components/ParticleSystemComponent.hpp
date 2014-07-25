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
#include <swift2d/geometries/HeatParticleEmitter.hpp>

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
  ParticleSystemComponent();

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
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "ParticleSystemComponent"; }

  void update(double time);

  void draw(RenderContext const& ctx);

  void serialize(SerializedScenePtr& scene) const;

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ParticleSystemPtr particle_system_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_PARTICLE_SYSTEM_COMPONENT_HPP
