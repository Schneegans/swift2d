////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_POINT_LIGHT_MATERIAL_HPP
#define SWIFT2D_POINT_LIGHT_MATERIAL_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Material.hpp>
#include <swift2d/materials/PointLightShader.hpp>
#include <swift2d/databases/TextureDatabase.hpp>
#include <swift2d/textures/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class PointLightMaterial;
typedef std::shared_ptr<PointLightMaterial>       PointLightMaterialPtr;
typedef std::shared_ptr<const PointLightMaterial> ConstPointLightMaterialPtr;
typedef Property<PointLightMaterialPtr>           PointLightMaterialProperty;

// -----------------------------------------------------------------------------
class PointLightMaterial : public Material {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  TextureProperty Texture;
  ColorProperty   Color;

  // ----------------------------------------------------- contruction interface
  PointLightMaterial()
    : Color(swift::Color(1.f, 1.f, 1.f)) {}

  // Creates a new material and returns a shared pointer.
  static PointLightMaterialPtr create() {
    return std::make_shared<PointLightMaterial>();
  }

  static PointLightMaterialPtr create_from_file(std::string const& file) {
    auto mat(std::make_shared<PointLightMaterial>());
    mat->Texture = Texture::create(file);
    return mat;
  }

  static PointLightMaterialPtr create_from_database(std::string const& id) {
    auto mat(std::make_shared<PointLightMaterial>());
    mat->Texture = TextureDatabase::instance()->get(id);
    return mat;
  }

  // creates a copy from this
  PointLightMaterialPtr create_copy() const {
    return std::make_shared<PointLightMaterial>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "PointLightMaterial"; }

  // uses the Material on the given context.
  /* virtual */ void use(RenderContext const& ctx,
                         math::mat3 const& object_transform) const {

    // disable rotation
    auto transform(math::make_translate(math::get_translate(object_transform)));
    transform = transform * math::make_scale(math::get_scale(object_transform));

    Texture()->bind(ctx, 0);
    PointLightShader::instance()->use(ctx);
    PointLightShader::instance()->set_uniform("projection", ctx.projection_matrix);
    PointLightShader::instance()->set_uniform("transform", transform);
    PointLightShader::instance()->set_uniform("screen_size", ctx.size);
    // PointLightShader::instance()->set_uniform("g_buffer_color", 1);
    PointLightShader::instance()->set_uniform("g_buffer_normal", 2);
    PointLightShader::instance()->set_uniform("g_buffer_aux", 3);
    PointLightShader::instance()->set_uniform("light_tex", 0);
    PointLightShader::instance()->set_uniform("light_color", Color().vec3());
  }
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_POINT_LIGHT_MATERIAL_HPP
