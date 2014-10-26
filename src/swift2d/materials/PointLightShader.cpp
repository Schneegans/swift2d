////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/PointLightShader.hpp>

namespace swift {

  ////////////////////////////////////////////////////////////////////////////////

  PointLightShader::PointLightShader()
    : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "fullscreen_quad_vertex_shader"
    )",
    R"(
      // fragment shader -----------------------------------------------------
      @include "version"

      @include "gbuffer_input"

      uniform ivec2 screen_size;
      uniform vec3  light_pos_radius[100];
      uniform vec4  light_colors[100];
      uniform int   light_count;

      in vec2 texcoords;

      layout (location = 0) out vec4 fragLight;

      void main(void){
        vec3  light_info = get_light_info(texcoords);
        float emit       = light_info.r;
        vec4 output      = vec4(0);

        if (emit < 1) {
          vec3  normal     = get_normal(texcoords);
          float gloss      = light_info.g;
          vec2  frag_pos   = (2.0*gl_FragCoord.xy/screen_size.xy)-1.0;

          for (int i=0; i<light_count; ++i) {
            vec2 dir = (frag_pos - light_pos_radius[i].xy) * screen_size / screen_size.x;
            float attenuation = max(0, (light_pos_radius[i].z - dir.x*dir.x + dir.y*dir.y) / light_pos_radius[i].z);

            vec3 light_dir = normalize(vec3(dir, -1));
            float specular = attenuation * max(0, pow(dot(normal, light_dir), gloss*100 + 1) * gloss);
            float diffuse  = attenuation * max(0, dot(light_dir, normal));
            output        += (1-emit) * vec4(diffuse*light_colors[i].rgb, specular) * light_colors[i].a;
          }
        }

        fragLight = output;
      }
    )"
    )
    , g_buffer_normal(get_uniform<int>("g_buffer_normal"))
    , g_buffer_light(get_uniform<int>("g_buffer_light"))
    , light_count(get_uniform<int>("light_count"))
    , light_colors(get_uniform<math::vec4>("light_colors"))
    , light_pos_radius(get_uniform<math::vec3>("light_pos_radius"))
    , screen_size(get_uniform<math::vec2i>("screen_size")) {}

  ////////////////////////////////////////////////////////////////////////////////

}
