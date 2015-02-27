////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATERIAL_SHADER_FACTORY_HPP
#define SWIFT2D_MATERIAL_SHADER_FACTORY_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/MaterialShader.hpp>
#include <swift2d/utils/Singleton.hpp>

#include <unordered_map>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class MaterialShaderFactory : public Singleton<MaterialShaderFactory> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  enum Capabilities {
    ANIMATED_DIFFUSE_TEX      = 1 << 0,
    DIFFUSE_TEX               = 1 << 1,
    ANIMATED_NORMAL_TEX       = 1 << 2,
    NORMAL_TEX                = 1 << 3,
    ANIMATED_EMIT_TEX         = 1 << 4,
    EMIT_TEX                  = 1 << 5,
    ANIMATED_GLOW_TEX         = 1 << 6,
    GLOW_TEX                  = 1 << 7,
    ANIMATED_SHINYNESS_TEX    = 1 << 8,
    SHINYNESS_TEX             = 1 << 9
  };

  MaterialShaderPtr get_shader(int capabilities);

  friend class Singleton<MaterialShaderFactory>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  MaterialShaderFactory() {};
  ~MaterialShaderFactory() {};

  std::unordered_map<int, MaterialShaderPtr> shaders_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_MATERIAL_SHADER_FACTORY_HPP
