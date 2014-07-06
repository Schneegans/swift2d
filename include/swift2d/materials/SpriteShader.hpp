////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_SHADER_HPP
#define SWIFT2D_SPRITE_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

#include <unordered_map>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SpriteShader : public Singleton<SpriteShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  enum Capabilities {
    DIFFUSE_TEX = 1 << 0,
    NORMAL_TEX = 1 << 1,
    EMIT_TEX = 1 << 2,
    GLOW_TEX = 1 << 3,
    SHINYNESS_TEX = 1 << 4,
    REFLECTIVITY_TEX = 1 << 5
  };

  ShaderPtr get_shader(int capabilities);

  friend class Singleton<SpriteShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  SpriteShader() {};
  ~SpriteShader() {};

  std::unordered_map<int, ShaderPtr> shaders_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SPRITE_SHADER_HPP
