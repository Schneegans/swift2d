////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_NUMBER_PARTICLE_SHADER_HPP
#define SWIFT2D_NUMBER_PARTICLE_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class NumberParticleShader : public Shader,
                             public Singleton<NumberParticleShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  projection;
  oglplus::Lazy<oglplus::Uniform<int>>         font;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>  scale;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  p_start_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  p_mid_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  p_end_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  n_start_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  n_mid_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  n_end_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  glow_mid_life;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>  burn;

  friend class Singleton<NumberParticleShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  NumberParticleShader();
  ~NumberParticleShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_NUMBER_PARTICLE_SHADER_HPP
