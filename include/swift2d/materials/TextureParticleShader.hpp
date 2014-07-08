////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TEXTURE_PARTICLE_SHADER_HPP
#define SWIFT2D_TEXTURE_PARTICLE_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class TextureParticleShader : public Shader,
                              public Singleton<TextureParticleShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::LazyUniform<math::mat3>  projection;
  oglplus::LazyUniform<math::mat3>  transform;
  oglplus::LazyUniform<int>         diffuse;
  oglplus::LazyUniform<float>       start_scale;
  oglplus::LazyUniform<float>       end_scale;
  oglplus::LazyUniform<math::vec3>  start_color;
  oglplus::LazyUniform<math::vec3>  end_color;
  oglplus::LazyUniform<float>       start_opacity;
  oglplus::LazyUniform<float>       end_opacity;
  oglplus::LazyUniform<float>       start_glow;
  oglplus::LazyUniform<float>       end_glow;
  oglplus::LazyUniform<int>         enable_rotation;

  friend class Singleton<TextureParticleShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  TextureParticleShader();
  ~TextureParticleShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_TEXTURE_PARTICLE_SHADER_HPP
