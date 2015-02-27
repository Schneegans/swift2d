////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_POINT_PARTICLE_SYSTEM_COMPONENT_HPP
#define SWIFT2D_POINT_PARTICLE_SYSTEM_COMPONENT_HPP

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
class PointParticleSystemComponent;
typedef std::shared_ptr<PointParticleSystemComponent>       PointParticleSystemComponentPtr;
typedef std::shared_ptr<const PointParticleSystemComponent> ConstPointParticleSystemComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL PointParticleSystemComponent : public ParticleSystemComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public ParticleSystemComponent::Serialized {
    float      MidLife;
    float      Scale;
    float      StartGlow,  MidGlow,  EndGlow;
    float      StartBurn,  MidBurn,  EndBurn;
    math::vec4 StartColor, MidColor, EndColor;
  };

  class Renderer : public ResourceRenderer<PointParticleSystemComponent> {
    void predraw(RenderContext const& ctx);
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  Float         MidLife;
  Float         Scale;
  Float         StartGlow,  MidGlow,  EndGlow;
  Float         StartBurn,  MidBurn,  EndBurn;
  ColorProperty StartColor, MidColor, EndColor;

  // ----------------------------------------------------- contruction interface
  PointParticleSystemComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static PointParticleSystemComponentPtr create(Args&& ... a) {
    return std::make_shared<PointParticleSystemComponent>(a...);
  }

  // creates a copy from this
  PointParticleSystemComponentPtr create_copy() const {
    return std::make_shared<PointParticleSystemComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "PointParticleSystemComponent"; }

  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_POINT_PARTICLE_SYSTEM_COMPONENT_HPP
