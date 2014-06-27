////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LIGHT_PARTICLE_EMITTER_HPP
#define SWIFT2D_LIGHT_PARTICLE_EMITTER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/ParticleEmitter.hpp>

#include <stack>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class LightParticleEmitter;
typedef std::shared_ptr<LightParticleEmitter>       LightParticleEmitterPtr;
typedef std::shared_ptr<const LightParticleEmitter> ConstLightParticleEmitterPtr;
typedef Property<LightParticleEmitterPtr>           LightParticleEmitterProperty;

// -----------------------------------------------------------------------------
class LightParticleEmitter : public ParticleEmitter {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  TextureProperty Texture;

  // ----------------------------------------------------- contruction interface
  LightParticleEmitter();

  // Creates a new component and returns a shared pointer.
  static LightParticleEmitterPtr create() {
    return std::make_shared<LightParticleEmitter>();
  }

  // creates a copy from this
  LightParticleEmitterPtr create_copy() const {
    return std::make_shared<LightParticleEmitter>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "LightParticleEmitter"; }

  bool serialize_as_light() const { return true; }

  void draw(RenderContext const& ctx, math::mat3 const& transform, int count) const;

  void spawn(math::mat3 transform, math::vec2& position, float& max_age,
             math::vec2& direction, float& rotation, float& rotation_speed) const;

  virtual void accept(SavableObjectVisitor& visitor) {
    ParticleEmitter::accept(visitor);
    visitor.add_object("Texture", Texture);
  }
};

}

#endif // SWIFT2D_LIGHT_PARTICLE_EMITTER_HPP
