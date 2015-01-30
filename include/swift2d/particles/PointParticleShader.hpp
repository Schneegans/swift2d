////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_POINT_PARTICLE_SHADER_HPP
#define SWIFT2D_POINT_PARTICLE_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class PointParticleShader : public Shader,
                             public Singleton<PointParticleShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  projection;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  start_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  mid_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  end_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  glow_mid_life;

  friend class Singleton<PointParticleShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  PointParticleShader();
  ~PointParticleShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_POINT_PARTICLE_SHADER_HPP
