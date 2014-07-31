////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DIRECTIONAL_LIGHT_MATERIAL_HPP
#define SWIFT2D_DIRECTIONAL_LIGHT_MATERIAL_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/LightMaterial.hpp>
#include <swift2d/materials/DirectionalLightShader.hpp>
#include <swift2d/databases/TextureDatabase.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class DirectionalLightMaterial;
typedef std::shared_ptr<DirectionalLightMaterial>       DirectionalLightMaterialPtr;
typedef std::shared_ptr<const DirectionalLightMaterial> ConstDirectionalLightMaterialPtr;
typedef Property<DirectionalLightMaterialPtr>           DirectionalLightMaterialProperty;

// -----------------------------------------------------------------------------
class DirectionalLightMaterial : public LightMaterial {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Vec3          Direction;
  ColorProperty Color;

  // ----------------------------------------------------- contruction interface
  DirectionalLightMaterial(math::vec3 const& dir = math::vec3(0.f, 0.f, -1.f),
                           swift::Color const& color = swift::Color(1.f, 1.f, 1.f));

  // Creates a new material and returns a shared pointer.
  template <typename... Args>
  static DirectionalLightMaterialPtr create(Args&& ... a) {
    return std::make_shared<DirectionalLightMaterial>(a...);
  }

  // creates a copy from this
  DirectionalLightMaterialPtr create_copy() const {
    return std::make_shared<DirectionalLightMaterial>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "DirectionalLightMaterial"; }

  // uses the Material on the given context.
  /* virtual */ void draw_quad(RenderContext const& ctx,
                               math::mat3 const& object_transform,
                               float object_depth);

  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_DIRECTIONAL_LIGHT_MATERIAL_HPP
