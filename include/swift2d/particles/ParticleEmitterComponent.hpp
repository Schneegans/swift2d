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
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/particles/ParticleSystemComponent.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ParticleEmitterComponent;
typedef std::shared_ptr<ParticleEmitterComponent>       ParticleEmitterComponentPtr;
typedef std::shared_ptr<const ParticleEmitterComponent> ConstParticleEmitterComponentPtr;
typedef Property<ParticleEmitterComponentPtr>           ParticleEmitterComponentProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL ParticleEmitterComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  ParticleSystemComponentPtr ParticleSystem;
  String                     ParticleSystemLabel;
  AnimatedFloat              Density;
  Vec2                       Velocity;

  // ----------------------------------------------------- contruction interface
  ParticleEmitterComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static ParticleEmitterComponentPtr create(Args&& ... a) {
    return std::make_shared<ParticleEmitterComponent>(a...);
  }

  // creates a copy from this
  ParticleEmitterComponentPtr create_copy() const {
    return std::make_shared<ParticleEmitterComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "ParticleEmitterComponent"; }

  virtual void update(double time);
  virtual void accept(SavableObjectVisitor& visitor);

 private:
  float particles_to_spawn_;
};

}

#endif // SWIFT2D_PARTICLE_EMITTER_HPP
