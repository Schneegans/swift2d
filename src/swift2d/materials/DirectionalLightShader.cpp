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
      @include "write_lbuffer"
      @include "light_helpers"

      void main(void){
        vec3 diffuse      = get_diffuse();
        float emit        = get_emit();
        vec3 normal       = get_normal();
        vec3 surface_dir  = normalize(normal - 0.5);

        float specular_light = get_specular_light(light_dir, surface_dir);
        float diffuse_light  = get_diffuse_light(light_dir, surface_dir);

        write_lbuffer((1 - emit) * (diffuse_light*diffuse + specular_light) * light_color.rgb);
      }
    )"
  )
  , screen_size(get_uniform<math::vec2i>("screen_size"))
  , g_buffer_diffuse(get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_normal(get_uniform<int>("g_buffer_normal"))
  , g_buffer_aux_1(get_uniform<int>("g_buffer_aux_1"))
  , light_dir(get_uniform<math::vec3>("light_dir"))
  , light_color(get_uniform<math::vec3>("light_color")) {}

////////////////////////////////////////////////////////////////////////////////

}
