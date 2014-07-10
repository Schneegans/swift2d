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
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  transform;
  oglplus::Lazy<oglplus::Uniform<int>>         heat_tex;
  oglplus::Lazy<oglplus::Uniform<float>>       start_scale;
  oglplus::Lazy<oglplus::Uniform<float>>       end_scale;
  oglplus::Lazy<oglplus::Uniform<float>>       start_opacity;
  oglplus::Lazy<oglplus::Uniform<float>>       end_opacity;

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
