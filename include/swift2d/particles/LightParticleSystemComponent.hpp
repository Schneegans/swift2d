////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LIGHT_PARTICLE_SYSTEM_COMPONENT_HPP
#define SWIFT2D_LIGHT_PARTICLE_SYSTEM_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleSystemComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class LightParticleSystemComponent;
typedef std::shared_ptr<LightParticleSystemComponent>       LightParticleSystemComponentPtr;
typedef std::shared_ptr<const LightParticleSystemComponent> ConstLightParticleSystemComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL LightParticleSystemComponent : public ParticleSystemComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public ParticleSystemComponent::Serialized {
    float      MidLife;
    float      StartScale, MidScale, EndScale;
    math::vec4 StartColor, MidColor, EndColor;
    TexturePtr Texture;
  };

  class Renderer : public ResourceRenderer<LightParticleSystemComponent> {
    void predraw(RenderContext const& ctx);
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  Float           MidLife;
  Float           StartScale, MidScale, EndScale;
  ColorProperty   StartColor, MidColor, EndColor;
  TextureProperty Texture;

  // ----------------------------------------------------- contruction interface
  LightParticleSystemComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static LightParticleSystemComponentPtr create(Args&& ... a) {
    return std::make_shared<LightParticleSystemComponent>(a...);
  }

  // creates a copy from this
  LightParticleSystemComponentPtr create_copy() const {
    return std::make_shared<LightParticleSystemComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "LightParticleSystemComponent"; }

  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_LIGHT_PARTICLE_SYSTEM_COMPONENT_HPP
