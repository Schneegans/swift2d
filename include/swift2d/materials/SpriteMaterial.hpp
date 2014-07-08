////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_MATERIAL_HPP
#define SWIFT2D_SPRITE_MATERIAL_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Material.hpp>
#include <swift2d/materials/SpriteShader.hpp>
#include <swift2d/utils/Color.hpp>
#include <swift2d/textures/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SpriteMaterial;
typedef std::shared_ptr<SpriteMaterial>       SpriteMaterialPtr;
typedef std::shared_ptr<const SpriteMaterial> ConstSpriteMaterialPtr;
typedef Property<SpriteMaterialPtr>           SpriteMaterialProperty;

// -----------------------------------------------------------------------------
class SpriteMaterial : public Material {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  TextureProperty DiffuseTexture;
  ColorProperty   Diffuse;

  TextureProperty NormalTexture;

  TextureProperty EmitTexture;
  Float           Emit;

  TextureProperty GlowTexture;
  Float           Glow;

  TextureProperty ShinynessTexture;
  Float           Shinyness;

  TextureProperty ReflectivityTexture;
  Float           Reflectivity;

  Bool            BlendAdditive;

  // ----------------------------------------------------- contruction interface
  SpriteMaterial();

  // Creates a new material and returns a shared pointer.
  static SpriteMaterialPtr create() {
    return std::make_shared<SpriteMaterial>();
  }

  // creates a copy from this
  SpriteMaterialPtr create_copy() const {
    return std::make_shared<SpriteMaterial>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "SpriteMaterial"; }

  // uses the Material on the given context.
  /* virtual */ void draw_quad(RenderContext const& ctx,
                               math::mat3 const& object_transform,
                               float object_depth);

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  bool            current_shader_dirty_;
  SpriteShaderPtr current_shader_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SPRITE_MATERIAL_HPP
