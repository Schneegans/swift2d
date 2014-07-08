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
  oglplus::LazyUniform<math::mat3>  projection;
  oglplus::LazyUniform<math::mat3>  transform;
  oglplus::LazyUniform<float>       depth;
  oglplus::LazyUniform<float>       parallax;
  oglplus::LazyUniform<int>         diffuse_tex;
  oglplus::LazyUniform<math::vec3>  diffuse;
  oglplus::LazyUniform<int>         normal_tex;
  oglplus::LazyUniform<math::mat3>  normal_transform;
  oglplus::LazyUniform<int>         emit_tex;
  oglplus::LazyUniform<float>       emit;
  oglplus::LazyUniform<int>         glow_tex;
  oglplus::LazyUniform<float>       glow;
  oglplus::LazyUniform<int>         shinyness_tex;
  oglplus::LazyUniform<float>       shinyness;
  oglplus::LazyUniform<int>         reflectivity_tex;
  oglplus::LazyUniform<float>       reflectivity;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SPRITE_SHADER_HPP
