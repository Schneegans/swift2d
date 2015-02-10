////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_NUMBER_PARTICLE_SYSTEM_COMPONENT_HPP
#define SWIFT2D_NUMBER_PARTICLE_SYSTEM_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleSystemComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>
#include <swift2d/graphics/SubSampler.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class NumberParticleSystemComponent;
typedef std::shared_ptr<NumberParticleSystemComponent>       NumberParticleSystemComponentPtr;
typedef std::shared_ptr<const NumberParticleSystemComponent> ConstNumberParticleSystemComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL NumberParticleSystemComponent : public ParticleSystemComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public ParticleSystemComponent::Serialized {
    float       MidLife;
    float       StartScale, MidScale, EndScale;
    float       StartGlow,  MidGlow,  EndGlow;
    float       StartBurn,  MidBurn,  EndBurn;
    float       ScaleBoost;
    math::int32 LowerBoostLimit, UpperBoostLimit;
    math::vec4  PositiveStartColor, PositiveMidColor, PositiveEndColor;
    math::vec4  NegativeStartColor, NegativeMidColor, NegativeEndColor;
    TexturePtr  Font;
  };

  class Renderer : public ResourceRenderer<NumberParticleSystemComponent> {
    void predraw(RenderContext const& ctx);
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  Float           MidLife;
  Float           StartScale, MidScale, EndScale;
  Float           StartGlow,  MidGlow,  EndGlow;
  Float           StartBurn,  MidBurn,  EndBurn;
  Float           ScaleBoost;
  Int32           LowerBoostLimit, UpperBoostLimit;
  ColorProperty   PositiveStartColor, PositiveMidColor, PositiveEndColor;
  ColorProperty   NegativeStartColor, NegativeMidColor, NegativeEndColor;
  TextureProperty Font;

  // ----------------------------------------------------- contruction interface
  NumberParticleSystemComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static NumberParticleSystemComponentPtr create(Args&& ... a) {
    return std::make_shared<NumberParticleSystemComponent>(a...);
  }

  // creates a copy from this
  NumberParticleSystemComponentPtr create_copy() const {
    return std::make_shared<NumberParticleSystemComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "NumberParticleSystemComponent"; }

  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_NUMBER_PARTICLE_SYSTEM_COMPONENT_HPP
