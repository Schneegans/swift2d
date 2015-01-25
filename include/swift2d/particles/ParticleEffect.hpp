////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PARTICLE_EFFECT_HPP
#define SWIFT2D_PARTICLE_EFFECT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ParticleEffect;
typedef std::shared_ptr<ParticleEffect>       ParticleEffectPtr;
typedef std::shared_ptr<const ParticleEffect> ConstParticleEffectPtr;
typedef Property<ParticleEffectPtr>           ParticleEffectProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL ParticleEffect : public SceneObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  AnimatedFloat Time;

  // ----------------------------------------------------- contruction interface
  static ParticleEffectPtr create() {
    return std::make_shared<ParticleEffect>();
  }

  ParticleEffect();

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "ParticleEffect"; }

  virtual void on_init();

  virtual void update(double time);
  virtual void accept(SavableObjectVisitor& visitor);
};

}

#endif // SWIFT2D_PARTICLE_EFFECT_HPP
