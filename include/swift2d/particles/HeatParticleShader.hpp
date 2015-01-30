////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_HEAT_PARTICLE_SHADER_HPP
#define SWIFT2D_HEAT_PARTICLE_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class HeatParticleShader : public Shader,
                           public Singleton<HeatParticleShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  projection;
  oglplus::Lazy<oglplus::Uniform<int>>         diffuse;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  scale_mid_life;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>  opacity;

  friend class Singleton<HeatParticleShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  HeatParticleShader();
  ~HeatParticleShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_HEAT_PARTICLE_SHADER_HPP
