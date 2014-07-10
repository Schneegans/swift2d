////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_HEAT_PARTICLE_EMITTER_HPP
#define SWIFT2D_HEAT_PARTICLE_EMITTER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/LightParticleEmitter.hpp>

#include <stack>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class HeatParticleEmitter;
typedef std::shared_ptr<HeatParticleEmitter>       HeatParticleEmitterPtr;
typedef std::shared_ptr<const HeatParticleEmitter> ConstHeatParticleEmitterPtr;
typedef Property<HeatParticleEmitterPtr>           HeatParticleEmitterProperty;

// -----------------------------------------------------------------------------
class HeatParticleEmitter : public LightParticleEmitter {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  // Creates a new component and returns a shared pointer.
  static HeatParticleEmitterPtr create() {
    return std::make_shared<HeatParticleEmitter>();
  }

  // creates a copy from this
  HeatParticleEmitterPtr create_copy() const {
    return std::make_shared<HeatParticleEmitter>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "HeatParticleEmitter"; }

  virtual SerializedScene::Target target() const { return SerializedScene::HEAT_OBJECTS; }

  virtual void draw(RenderContext const& ctx, math::mat3 const& transform, int count) const;

  virtual void accept(SavableObjectVisitor& visitor) {
    LightParticleEmitter::accept(visitor);
  }
};

}

#endif // SWIFT2D_HEAT_PARTICLE_EMITTER_HPP
