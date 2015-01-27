////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PARTICLE_UPDATE_SHADER_HPP
#define SWIFT2D_PARTICLE_UPDATE_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class ParticleUpdateShader : public Shader,
                             public Singleton<ParticleUpdateShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::vec2>>   time;
  oglplus::Lazy<oglplus::Uniform<int>>          noise_tex;
  oglplus::Lazy<oglplus::Uniform<math::vec2i>>  spawn_count_it;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>   transform;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>   life;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>   dir_pos_rot_variance;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>   velocity;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>   rotation;
  oglplus::Lazy<oglplus::Uniform<int>>          gravity_map;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>   projection;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>   dynamics;

  friend class Singleton<ParticleUpdateShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  ParticleUpdateShader();
  ~ParticleUpdateShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_PARTICLE_UPDATE_SHADER_HPP
