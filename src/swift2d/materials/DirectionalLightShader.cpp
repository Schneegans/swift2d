////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/DirectionalLightShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DirectionalLightShader::DirectionalLightShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "fullscreen_quad_vertext_shader"
    )",
    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      // uniforms
      uniform vec3 light_dir;
      uniform vec3 light_color;

      @include "gbuffer_input"
      @include "get_lit_surface_color"

      layout (location = 0) out vec3 fragColor;

      void main(void){
        fragColor = get_lit_surface_color(light_dir, light_color, 1.0);
      }
    )"
  )
  , screen_size(get_uniform<math::vec2i>("screen_size"))
  , g_buffer_diffuse(get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_normal(get_uniform<int>("g_buffer_normal"))
  , g_buffer_light(get_uniform<int>("g_buffer_light"))
  , light_dir(get_uniform<math::vec3>("light_dir"))
  , light_color(get_uniform<math::vec3>("light_color")) {}

////////////////////////////////////////////////////////////////////////////////

}
