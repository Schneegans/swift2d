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
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  projection;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  transform;
  oglplus::Lazy<oglplus::Uniform<int>>         diffuse;
  oglplus::Lazy<oglplus::Uniform<float>>       start_scale;
  oglplus::Lazy<oglplus::Uniform<float>>       end_scale;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>  start_color;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>  end_color;
  oglplus::Lazy<oglplus::Uniform<float>>       start_opacity;
  oglplus::Lazy<oglplus::Uniform<float>>       end_opacity;
  oglplus::Lazy<oglplus::Uniform<float>>       start_glow;
  oglplus::Lazy<oglplus::Uniform<float>>       end_glow;
  oglplus::Lazy<oglplus::Uniform<int>>         enable_rotation;

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
