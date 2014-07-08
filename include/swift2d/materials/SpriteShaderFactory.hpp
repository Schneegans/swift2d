////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_SHADER_FACTORY_HPP
#define SWIFT2D_SPRITE_SHADER_FACTORY_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/SpriteShader.hpp>
#include <swift2d/utils/Singleton.hpp>

#include <unordered_map>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SpriteShaderFactory : public Singleton<SpriteShaderFactory> {

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

  SpriteShaderPtr get_shader(int capabilities);

  friend class Singleton<SpriteShaderFactory>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  SpriteShaderFactory() {};
  ~SpriteShaderFactory() {};

  std::unordered_map<int, SpriteShaderPtr> shaders_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SPRITE_SHADER_FACTORY_HPP
