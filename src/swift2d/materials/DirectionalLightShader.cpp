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

// includes  -------------------------------------------------------------------
#include <swift2d/materials/DirectionalLightShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DirectionalLightShader::DirectionalLightShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "fullscreen_quad_vertex_shader"
    )",
    R"(
      // fragment shader -----------------------------------------------------
      @include "version"

      @include "gbuffer_input"

      uniform vec3 light_dirs[10];
      uniform vec4 light_colors[10];
      uniform int  light_count;

      in vec2 texcoords;

      layout (location = 0) out vec4 fragLight;

      void main(void){
        vec3  light_info = get_light_info(texcoords);
        float emit       = light_info.r;
        vec4 result      = vec4(vec3(emit), 0);

        if (light_count > 0) {
          if (emit < 1) {
            vec3  normal     = get_normal(texcoords);
            float gloss      = light_info.g;

            for (int i=0; i<light_count; ++i) {
              float specular = max(0, pow(dot(normal, normalize(light_dirs[i] + vec3(0, 0, -1))), gloss*100 + 1) * gloss);
              float diffuse  = max(0, dot(light_dirs[i], normal));
              result        += (1-emit) * vec4(diffuse*light_colors[i].rgb, specular) * light_colors[i].a;
            }
          }
        }

        fragLight = result;
      }
    )"
  )
  , g_buffer_normal(get_uniform<int>("g_buffer_normal"))
  , g_buffer_light(get_uniform<int>("g_buffer_light"))
  , light_count(get_uniform<int>("light_count"))
  , light_colors(get_uniform<math::vec4>("light_colors"))
  , light_dirs(get_uniform<math::vec3>("light_dirs")) {}

////////////////////////////////////////////////////////////////////////////////

}
