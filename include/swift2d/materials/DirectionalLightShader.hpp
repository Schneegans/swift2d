////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
  oglplus::Lazy<oglplus::Uniform<math::vec2i>>  screen_size;
  oglplus::Lazy<oglplus::Uniform<int>>          g_buffer_diffuse;
  oglplus::Lazy<oglplus::Uniform<int>>          g_buffer_normal;
  oglplus::Lazy<oglplus::Uniform<int>>          g_buffer_light;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>   light_dir;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>   light_color;

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
