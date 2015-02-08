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
  oglplus::Lazy<oglplus::Uniform<math::vec2i>>  spawn_count_it_collision_mode;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>   position;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>   emitter_velocity;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>   life_pos_var;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>   lin_ang_velocity;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>   direction_rotation;
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
