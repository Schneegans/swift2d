////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_BUMP_TEXTURE_MATERIAL_HPP
#define SWIFT2D_BUMP_TEXTURE_MATERIAL_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Material.hpp>
#include <swift2d/materials/BumpTextureShader.hpp>
#include <swift2d/textures/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class BumpTextureMaterial;
typedef std::shared_ptr<BumpTextureMaterial>       BumpTextureMaterialPtr;
typedef std::shared_ptr<const BumpTextureMaterial> ConstBumpTextureMaterialPtr;
typedef Property<BumpTextureMaterialPtr>           BumpTextureMaterialProperty;

// -----------------------------------------------------------------------------
class BumpTextureMaterial : public Material {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  TextureProperty Texture;
  TextureProperty NormalMap;
  Float           Emit;
  Float           Shinyness;
  Float           Reflectivity;

  // ----------------------------------------------------- contruction interface
  // Creates a new material and returns a shared pointer.
  static BumpTextureMaterialPtr create() {
    return std::make_shared<BumpTextureMaterial>();
  }

  static BumpTextureMaterialPtr create_from_files(std::string const& texture,
                                                  std::string const& normal_texture,
                                                  float emit = 0.f,
                                                  float shinyness = 10.f,
                                                  float reflectivity = 0.5f) {
    auto mat(std::make_shared<BumpTextureMaterial>());
    mat->Texture = Texture::create(texture);
    mat->NormalMap = Texture::create(normal_texture);
    mat->Emit = emit;
    mat->Shinyness = shinyness;
    mat->Reflectivity = reflectivity;
    return mat;
  }

  static BumpTextureMaterialPtr create_from_database(std::string const& id,
                                                     std::string const& normal_id,
                                                     float emit = 0.f,
                                                     float shinyness = 10.f,
                                                     float reflectivity = 0.5f) {
    auto mat(std::make_shared<BumpTextureMaterial>());
    mat->Texture = TextureDatabase::instance()->get(id);
    mat->NormalMap = TextureDatabase::instance()->get(normal_id);
    mat->Emit = emit;
    mat->Shinyness = shinyness;
    mat->Reflectivity = reflectivity;
    return mat;
  }

  // creates a copy from this
  BumpTextureMaterialPtr create_copy() const {
    return std::make_shared<BumpTextureMaterial>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "BumpTextureMaterial"; }

  // uses the Material on the given context.
  /* virtual */ void use(RenderContext const& ctx,
                         math::mat3 const& object_transform) const {
    Texture()->bind(ctx, 0);
    NormalMap()->bind(ctx, 1);
    BumpTextureShader::instance()->use(ctx);
    BumpTextureShader::instance()->set_uniform("projection", ctx.projection_matrix);
    BumpTextureShader::instance()->set_uniform("transform", object_transform);
    BumpTextureShader::instance()->set_uniform("normal_transform", math::transposed(math::inversed(object_transform)));
    BumpTextureShader::instance()->set_uniform("diffuse", 0);
    BumpTextureShader::instance()->set_uniform("normal", 1);
    BumpTextureShader::instance()->set_uniform("emit", Emit());
    BumpTextureShader::instance()->set_uniform("shinyness", Shinyness());
    BumpTextureShader::instance()->set_uniform("reflectivity", Reflectivity());
  }
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_BUMP_TEXTURE_MATERIAL_HPP
