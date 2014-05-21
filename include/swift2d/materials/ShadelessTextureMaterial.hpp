////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SHADELESS_TEXTURE_MATERIAL_HPP
#define SWIFT2D_SHADELESS_TEXTURE_MATERIAL_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Material.hpp>
#include <swift2d/materials/ShadelessTextureShader.hpp>
#include <swift2d/resources/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ShadelessTextureMaterial;
typedef std::shared_ptr<ShadelessTextureMaterial>       ShadelessTextureMaterialPtr;
typedef std::shared_ptr<const ShadelessTextureMaterial> ConstShadelessTextureMaterialPtr;
typedef Property<ShadelessTextureMaterialPtr>           ShadelessTextureMaterialProperty;

// -----------------------------------------------------------------------------
class ShadelessTextureMaterial : public Material {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  TextureProperty Texture;

  // ----------------------------------------------------- contruction interface
  // Creates a new material and returns a shared pointer.
  static ShadelessTextureMaterialPtr create() {
    return std::make_shared<ShadelessTextureMaterial>();
  }

  static ShadelessTextureMaterialPtr create_from_file(std::string const& file) {
    auto mat(std::make_shared<ShadelessTextureMaterial>());
    mat->Texture = Texture::create(file);
    return mat;
  }

  static ShadelessTextureMaterialPtr create_from_database(std::string const& id) {
    auto mat(std::make_shared<ShadelessTextureMaterial>());
    mat->Texture = TextureDatabase::instance()->get(id);
    return mat;
  }

  // creates a copy from this
  ShadelessTextureMaterialPtr create_copy() const {
    return std::make_shared<ShadelessTextureMaterial>(*this);
  }

  // ------------------------------------------------------------ public methods
  // uses the Material on the given context.
  /* virtual */ void use(RenderContext const& ctx,
                         math::mat3 const& object_transform) const {
    Texture()->bind(ctx, 0);
    ShadelessTextureShader::instance()->use(ctx);
    ShadelessTextureShader::instance()->set_uniform("projection", ctx.projection_matrix);
    ShadelessTextureShader::instance()->set_uniform("transform", object_transform);
    ShadelessTextureShader::instance()->set_uniform("diffuse", 0);
  }
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SHADELESS_TEXTURE_MATERIAL_HPP
