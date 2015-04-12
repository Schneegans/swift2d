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

#ifndef SWIFT2D_POINT_LIGHT_SHADER_HPP
#define SWIFT2D_POINT_LIGHT_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class PointLightShader : public Shader,
                         public Singleton<PointLightShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::mat3>>   projection;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>   transform;
  oglplus::Lazy<oglplus::Uniform<float>>        depth;
  oglplus::Lazy<oglplus::Uniform<float>>        parallax;
  oglplus::Lazy<oglplus::Uniform<math::vec2i>>  screen_size;
  oglplus::Lazy<oglplus::Uniform<int>>          g_buffer_normal;
  oglplus::Lazy<oglplus::Uniform<int>>          g_buffer_light;
  oglplus::Lazy<oglplus::Uniform<int>>          light_tex;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>   light_color;

  friend class Singleton<PointLightShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  PointLightShader();
  ~PointLightShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_POINT_LIGHT_SHADER_HPP
