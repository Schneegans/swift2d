////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DIRECTIONAL_LIGHT_COMPONENT_HPP
#define SWIFT2D_DIRECTIONAL_LIGHT_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class DirectionalLightComponent;
typedef std::shared_ptr<DirectionalLightComponent>       DirectionalLightComponentPtr;
typedef std::shared_ptr<const DirectionalLightComponent> ConstDirectionalLightComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL DirectionalLightComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public SerializedComponent {
    math::vec3 Direction;
    math::vec4 Color;
  };

  class Renderer : public ResourceRenderer<DirectionalLightComponent> {
    void draw(RenderContext const& ctx, int start, int end);
    void draw_no_objects(RenderContext const& ctx);
  };

  // ---------------------------------------------------------------- properties
  Vec3          Direction;
  ColorProperty Color;

  // ----------------------------------------------------- contruction interface
  DirectionalLightComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static DirectionalLightComponentPtr create(Args&& ... a) {
    return std::make_shared<DirectionalLightComponent>(a...);
  }

  // creates a copy from this
  DirectionalLightComponentPtr create_copy() const {
    return std::make_shared<DirectionalLightComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "DirectionalLightComponent"; }

  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_DIRECTIONAL_LIGHT_COMPONENT_HPP
