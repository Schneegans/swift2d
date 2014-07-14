////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TEXTURE_PARTICLE_EMITTER_HPP
#define SWIFT2D_TEXTURE_PARTICLE_EMITTER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/ParticleEmitter.hpp>

#include <stack>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class TextureParticleEmitter;
typedef std::shared_ptr<TextureParticleEmitter>       TextureParticleEmitterPtr;
typedef std::shared_ptr<const TextureParticleEmitter> ConstTextureParticleEmitterPtr;
typedef Property<TextureParticleEmitterPtr>           TextureParticleEmitterProperty;

// -----------------------------------------------------------------------------
class TextureParticleEmitter : public ParticleEmitter {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Bool            BlendAdditive;

  Float           RotationSpeed;
  Float           RotationSpeedVariance;

  Float           Rotation;
  Float           RotationVariance;

  Float           StartGlow;
  Float           EndGlow;

  TextureProperty Texture;


  // ----------------------------------------------------- contruction interface
  TextureParticleEmitter();

  // Creates a new component and returns a shared pointer.
  static TextureParticleEmitterPtr create() {
    return std::make_shared<TextureParticleEmitter>();
  }

  // creates a copy from this
  TextureParticleEmitterPtr create_copy() const {
    return std::make_shared<TextureParticleEmitter>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "TextureParticleEmitter"; }

  virtual SerializedScene::Target target() const { return SerializedScene::OBJECTS; }

  void draw(RenderContext const& ctx, math::mat3 const& transform, int count) const;

  void spawn(math::mat3 transform, math::vec2& position, float& max_age,
             math::vec2& direction, float& rotation, float& rotation_speed) const;

  virtual void accept(SavableObjectVisitor& visitor);
};

}

#endif // SWIFT2D_TEXTURE_PARTICLE_EMITTER_HPP