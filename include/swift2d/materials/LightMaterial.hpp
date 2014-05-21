////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LIGHT_MATERIAL_HPP
#define SWIFT2D_LIGHT_MATERIAL_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Material.hpp>
#include <swift2d/materials/LightShader.hpp>
#include <swift2d/databases/TextureDatabase.hpp>
#include <swift2d/resources/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class LightMaterial;
typedef std::shared_ptr<LightMaterial>       LightMaterialPtr;
typedef std::shared_ptr<const LightMaterial> ConstLightMaterialPtr;
typedef Property<LightMaterialPtr>           LightMaterialProperty;

// -----------------------------------------------------------------------------
class LightMaterial : public Material {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  TextureProperty Texture;

  // ----------------------------------------------------- contruction interface
  // Creates a new material and returns a shared pointer.
  static LightMaterialPtr create() {
    return std::make_shared<LightMaterial>();
  }

  static LightMaterialPtr create_from_file(std::string const& file) {
    auto mat(std::make_shared<LightMaterial>());
    mat->Texture = Texture::create(file);
    return mat;
  }

  static LightMaterialPtr create_from_database(std::string const& id) {
    auto mat(std::make_shared<LightMaterial>());
    mat->Texture = TextureDatabase::instance()->get(id);
    return mat;
  }

  // creates a copy from this
  LightMaterialPtr create_copy() const {
    return std::make_shared<LightMaterial>(*this);
  }

  // ------------------------------------------------------------ public methods
  // uses the Material on the given context.
  /* virtual */ void use(RenderContext const& ctx,
                         math::mat3 const& object_transform) const {

    auto transform(math::make_translate(math::get_translate(object_transform)));
    transform = transform * math::make_scale(math::get_scale(object_transform));

    Texture()->bind(ctx, 0);
    LightShader::instance()->use(ctx);
    LightShader::instance()->set_common_uniforms(ctx, transform);
    LightShader::instance()->set_uniform("screen_size", ctx.size);
    LightShader::instance()->set_uniform("g_buffer_normal", 2);
    LightShader::instance()->set_uniform("light_tex", 0);
  }
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_LIGHT_MATERIAL_HPP
