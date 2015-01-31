////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_POINT_LIGHT_COMPONENT_HPP
#define SWIFT2D_POINT_LIGHT_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/components/DepthComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class PointLightComponent;
typedef std::shared_ptr<PointLightComponent>       PointLightComponentPtr;
typedef std::shared_ptr<const PointLightComponent> ConstPointLightComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL PointLightComponent : public TransformableComponent,
                                      public DepthComponent  {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public SerializedComponent {
    math::mat3 Transform;
    TexturePtr Texture;
    math::vec4 Color;
  };

  class Renderer : public ResourceRenderer<PointLightComponent> {
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  TextureProperty Texture;
  ColorProperty   Color;

  // ----------------------------------------------------- contruction interface
  PointLightComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static PointLightComponentPtr create(Args&& ... a) {
    return std::make_shared<PointLightComponent>(a...);
  }

  // creates a copy from this
  PointLightComponentPtr create_copy() const {
    return std::make_shared<PointLightComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "PointLightComponent"; }

  virtual void update(double time);
  virtual void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_POINT_LIGHT_COMPONENT_HPP
