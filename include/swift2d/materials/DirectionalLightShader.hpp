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

#ifndef SWIFT2D_DIRECTIONAL_LIGHT_SHADER_HPP
#define SWIFT2D_DIRECTIONAL_LIGHT_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class DirectionalLightShader : public Shader,
                               public Singleton<DirectionalLightShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<int>>          g_buffer_normal;
  oglplus::Lazy<oglplus::Uniform<int>>          g_buffer_light;
  oglplus::Lazy<oglplus::Uniform<int>>          light_count;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>   light_colors;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>   light_dirs;

  friend class Singleton<DirectionalLightShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  DirectionalLightShader();
  ~DirectionalLightShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_DIRECTIONAL_LIGHT_SHADER_HPP
