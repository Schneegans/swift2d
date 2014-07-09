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

// shared pointer type definition ----------------------------------------------
class SpriteShader;
typedef std::shared_ptr<SpriteShader>       SpriteShaderPtr;
typedef std::shared_ptr<const SpriteShader> ConstSpriteShaderPtr;

// -----------------------------------------------------------------------------
class SpriteShader : public Shader {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static SpriteShaderPtr create(Args&& ... a) {
    return std::make_shared<SpriteShader>(a...);
  }

  SpriteShader(int capabilities, std::string const& v_source,
               std::string const& f_source);

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  projection;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  transform;
  oglplus::Lazy<oglplus::Uniform<float>>       depth;
  oglplus::Lazy<oglplus::Uniform<float>>       parallax;
  oglplus::Lazy<oglplus::Uniform<int>>         diffuse_tex;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>  diffuse;
  oglplus::Lazy<oglplus::Uniform<int>>         normal_tex;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  normal_transform;
  oglplus::Lazy<oglplus::Uniform<int>>         emit_tex;
  oglplus::Lazy<oglplus::Uniform<float>>       emit;
  oglplus::Lazy<oglplus::Uniform<int>>         glow_tex;
  oglplus::Lazy<oglplus::Uniform<float>>       glow;
  oglplus::Lazy<oglplus::Uniform<int>>         shinyness_tex;
  oglplus::Lazy<oglplus::Uniform<float>>       shinyness;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SPRITE_SHADER_HPP
