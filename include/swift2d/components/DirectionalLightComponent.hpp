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
#include <swift2d/components/DrawableComponent.hpp>
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
class DirectionalLightComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float         Depth;
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

  void draw(RenderContext const& ctx);

  void serialize(SerializedScenePtr& scene) const;

  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_DIRECTIONAL_LIGHT_COMPONENT_HPP
