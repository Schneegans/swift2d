////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LIGHT_PARTICLE_SHADER_HPP
#define SWIFT2D_LIGHT_PARTICLE_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class LightParticleShader : public Shader,
                            public Singleton<LightParticleShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  projection;
  oglplus::Lazy<oglplus::Uniform<int>>         diffuse;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  scale_mid_life;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  start_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  mid_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  end_color;
  oglplus::Lazy<oglplus::Uniform<math::vec2i>> screen_size;
  oglplus::Lazy<oglplus::Uniform<int>>         g_buffer_normal;
  oglplus::Lazy<oglplus::Uniform<int>>         g_buffer_light;

  friend class Singleton<LightParticleShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  LightParticleShader();
  ~LightParticleShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_LIGHT_PARTICLE_SHADER_HPP
