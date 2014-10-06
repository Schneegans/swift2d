////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_POINT_PARTICLE_SYSTEM_COMPONENT_HPP
#define SWIFT2D_POINT_PARTICLE_SYSTEM_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleSystemComponent.hpp>
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
  struct Serialized : public ParticleSystemComponent::Serialized {
    float      Scale;
    float      StartGlow,  EndGlow;
    math::vec4 StartColor, EndColor;
    bool       BlendAdd;
  };

  // ---------------------------------------------------------------- properties
  Float         Scale;
  Float         StartGlow,  EndGlow;
  ColorProperty StartColor, EndColor;
  Bool          BlendAdd;

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

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "PointParticleSystemComponent"; }

  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_POINT_PARTICLE_SYSTEM_COMPONENT_HPP
