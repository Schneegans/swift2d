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
#include <swift2d/materials/Material.hpp>
#include <swift2d/materials/DirectionalLightShader.hpp>
#include <swift2d/databases/TextureDatabase.hpp>
#include <swift2d/textures/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class DirectionalLightMaterial;
typedef std::shared_ptr<DirectionalLightMaterial>       DirectionalLightMaterialPtr;
typedef std::shared_ptr<const DirectionalLightMaterial> ConstDirectionalLightMaterialPtr;
typedef Property<DirectionalLightMaterialPtr>           DirectionalLightMaterialProperty;

// -----------------------------------------------------------------------------
class DirectionalLightMaterial : public Material {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Vec3 Direction;

  // ----------------------------------------------------- contruction interface
  // Creates a new material and returns a shared pointer.
  static DirectionalLightMaterialPtr create() {
    return std::make_shared<DirectionalLightMaterial>();
  }

  static DirectionalLightMaterialPtr create(math::vec3 const& direction) {
    auto mat(std::make_shared<DirectionalLightMaterial>());
    mat->Direction = direction;
    return mat;
  }

  // creates a copy from this
  DirectionalLightMaterialPtr create_copy() const {
    return std::make_shared<DirectionalLightMaterial>(*this);
  }

  // ------------------------------------------------------------ public methods
  // uses the Material on the given context.
  /* virtual */ void use(RenderContext const& ctx,
                         math::mat3 const& object_transform) const {

    // disable rotation
    // auto transform(math::make_translate(math::get_translate(object_transform)));
    // transform = transform * math::make_scale(math::get_scale(object_transform));

    DirectionalLightShader::instance()->use(ctx);
    DirectionalLightShader::instance()->set_uniform("projection", math::mat3());
    DirectionalLightShader::instance()->set_uniform("transform", math::mat3());
    DirectionalLightShader::instance()->set_uniform("screen_size", ctx.size);
    DirectionalLightShader::instance()->set_uniform("g_buffer_normal", 2);
    DirectionalLightShader::instance()->set_uniform("light_dir", math::normalized(Direction()));
  }
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_DIRECTIONAL_LIGHT_MATERIAL_HPP
