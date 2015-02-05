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
#include <swift2d/materials/MaterialBase.hpp>
#include <swift2d/materials/MaterialShader.hpp>
#include <swift2d/utils/Color.hpp>
#include <swift2d/textures/Texture3D.hpp>
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
class SWIFT_DLL Material : public MaterialBase {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Texture3DProperty AnimatedDiffuseTexture;

  String            DiffuseTextureName;
  TextureProperty   DiffuseTexture;
  ColorProperty     Diffuse;

  Texture3DProperty AnimatedNormalTexture;

  String            NormalTextureName;
  TextureProperty   NormalTexture;

  Texture3DProperty AnimatedEmitTexture;

  String            EmitTextureName;
  TextureProperty   EmitTexture;
  Float             Emit;

  Texture3DProperty AnimatedGlowTexture;

  String            GlowTextureName;
  TextureProperty   GlowTexture;
  Float             Glow;

  Texture3DProperty AnimatedShinynessTexture;

  String            ShinynessTextureName;
  TextureProperty   ShinynessTexture;
  Float             Shinyness;

  Vec2              TexcoordOffset;
  Vec2              TexcoordScale;

  Bool              BlendAdditive;

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
  void draw_quad(
    RenderContext const& ctx, math::mat3 const& transform,
    float depth, float time = 0.f);

  void draw_quads(
    RenderContext const& ctx, std::vector<math::mat3> const& transforms,
    float depth, std::vector<float> const& times = {});

  void draw_fullscreen_quad(RenderContext const& ctx, float time = 0.f);

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void draw_quad_impl(RenderContext const& ctx, std::vector<math::mat3> const& transforms,
                      math::mat3 const& projection, float depth,
                      std::vector<float> const& times);

  bool              current_shader_dirty_;
  MaterialShaderPtr current_shader_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_MATERIAL_HPP
