////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TRAIL_UPDATE_SHADER_HPP
#define SWIFT2D_TRAIL_UPDATE_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class TrailUpdateShader : public Shader,
                          public Singleton<TrailUpdateShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::vec2>>   time;
  oglplus::Lazy<oglplus::Uniform<int>>          spawn_count;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>   transform;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>   prev_1_transform;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>   prev_2_transform;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>   prev_3_transform;
  oglplus::Lazy<oglplus::Uniform<float>>        life;

  friend class Singleton<TrailUpdateShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  TrailUpdateShader();
  ~TrailUpdateShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_TRAIL_UPDATE_SHADER_HPP
