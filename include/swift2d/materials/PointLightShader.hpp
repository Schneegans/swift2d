////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
   oglplus::Lazy<oglplus::Uniform<int>>          g_buffer_normal;
   oglplus::Lazy<oglplus::Uniform<int>>          g_buffer_light;
   oglplus::Lazy<oglplus::Uniform<int>>          light_count;
   oglplus::Lazy<oglplus::Uniform<math::vec4>>   light_colors;
   oglplus::Lazy<oglplus::Uniform<math::vec3>>   light_pos_radius;
   oglplus::Lazy<oglplus::Uniform<math::vec2i>>  screen_size;

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
