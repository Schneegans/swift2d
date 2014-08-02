////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATERIAL_HPP
#define SWIFT2D_MATERIAL_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/SavableObject.hpp>
#include <swift2d/materials/MaterialShader.hpp>
#include <swift2d/utils/Color.hpp>
#include <swift2d/textures/AnimatedTexture.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/events/Timer.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Material;
typedef std::shared_ptr<Material>       MaterialPtr;
typedef std::shared_ptr<const Material> ConstMaterialPtr;
typedef Property<MaterialPtr>           MaterialProperty;

// -----------------------------------------------------------------------------
class Material : public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  AnimatedTextureProperty AnimatedDiffuseTexture;
  TextureProperty         DiffuseTexture;
  ColorProperty           Diffuse;

  AnimatedTextureProperty AnimatedNormalTexture;
  TextureProperty         NormalTexture;

  AnimatedTextureProperty AnimatedEmitTexture;
  TextureProperty         EmitTexture;
  Float                   Emit;

  AnimatedTextureProperty AnimatedGlowTexture;
  TextureProperty         GlowTexture;
  Float                   Glow;

  AnimatedTextureProperty AnimatedShinynessTexture;
  TextureProperty         ShinynessTexture;
  Float                   Shinyness;

  Bool                    BlendAdditive;

  // ----------------------------------------------------- contruction interface
  Material();

  // Creates a new material and returns a shared pointer.
  static MaterialPtr create() {
    return std::make_shared<Material>();
  }

  // creates a copy from this
  MaterialPtr create_copy() const {
    return std::make_shared<Material>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Material"; }

  // uses the Material on the given context.
  void draw_quad(RenderContext const& ctx, math::mat3 const& object_transform,
                 float object_depth, float time = 0.f);

  void draw_fullscreen_quad(RenderContext const& ctx, float time = 0.f);

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void draw_quad_impl(RenderContext const& ctx, math::mat3 const& object_transform,
                      math::mat3 const& projection, float object_depth, float time);

  bool              current_shader_dirty_;
  MaterialShaderPtr current_shader_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_MATERIAL_HPP
