////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
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
  oglplus::Lazy<oglplus::Uniform<math::vec2>>   position;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>   times;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>   prev_1_position;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>   prev_2_position;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>   prev_3_position;
  oglplus::Lazy<oglplus::Uniform<int>>          use_global_texcoords;

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
