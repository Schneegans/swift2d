////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_PARTICLE_SYSTEM_COMPONENT_HPP
#define SWIFT2D_SPRITE_PARTICLE_SYSTEM_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/particles/ParticleSystem.hpp>
#include <swift2d/textures/Texture.hpp>

#include <unordered_set>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SpriteParticleSystemComponent;
typedef std::shared_ptr<SpriteParticleSystemComponent>       SpriteParticleSystemComponentPtr;
typedef std::shared_ptr<const SpriteParticleSystemComponent> ConstSpriteParticleSystemComponentPtr;

// -----------------------------------------------------------------------------
class SpriteParticleSystemComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float           Depth;
  TextureProperty Texture;

  // ----------------------------------------------------- contruction interface
  SpriteParticleSystemComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static SpriteParticleSystemComponentPtr create(Args&& ... a) {
    return std::make_shared<SpriteParticleSystemComponent>(a...);
  }

  // creates a copy from this
  SpriteParticleSystemComponentPtr create_copy() const {
    return std::make_shared<SpriteParticleSystemComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "SpriteParticleSystemComponent"; }

  void add_emitter(ParticleEmitterComponentPtr const& emitter);
  void remove_emitter(ParticleEmitterComponentPtr const& emitter);

  void update(double time);
  void draw(RenderContext const& ctx);
  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ParticleSystemPtr particle_system_;

  std::unordered_set<ParticleEmitterComponentPtr> emitters_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_SPRITE_PARTICLE_SYSTEM_COMPONENT_HPP
