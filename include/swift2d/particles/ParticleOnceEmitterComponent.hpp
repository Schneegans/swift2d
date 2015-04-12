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

#ifndef SWIFT2D_PARTICLE_ONCE_EMITTER_HPP
#define SWIFT2D_PARTICLE_ONCE_EMITTER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/particles/ParticleSystemComponent.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ParticleOnceEmitterComponent;
typedef std::shared_ptr<ParticleOnceEmitterComponent>       ParticleOnceEmitterComponentPtr;
typedef std::shared_ptr<const ParticleOnceEmitterComponent> ConstParticleOnceEmitterComponentPtr;
typedef Property<ParticleOnceEmitterComponentPtr>           ParticleOnceEmitterComponentProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL ParticleOnceEmitterComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  ParticleSystemComponentPtr ParticleSystem;
  String                     ParticleSystemLabel;
  Double                     Delay;
  Float                      Amount;
  Bool                       DetachOnEmmission;

  // ----------------------------------------------------- contruction interface
  ParticleOnceEmitterComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static ParticleOnceEmitterComponentPtr create(Args&& ... a) {
    return std::make_shared<ParticleOnceEmitterComponent>(a...);
  }

  // creates a copy from this
  ParticleOnceEmitterComponentPtr create_copy() const {
    return std::make_shared<ParticleOnceEmitterComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "ParticleOnceEmitterComponent"; }

  void restart();

  virtual void update(double time);
  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  double age_;
};

}

#endif // SWIFT2D_PARTICLE_ONCE_EMITTER_HPP
