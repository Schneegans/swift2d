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
#include <swift2d/materials/PointLightShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointLightShader::PointLightShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "instanced_quad_vertex_shader"
    )",
    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      // varyings
      in vec2 texcoords;
      flat in int instance_id;

      // uniforms
      uniform sampler2D light_tex;
      uniform vec4      light_color[100];
      uniform ivec2     screen_size;

      @include "gbuffer_input"
      @include "get_light"

      layout (location = 0) out vec4 fragLight;

      void main(void){

        vec3  light_info  = get_light_info(gl_FragCoord.xy/screen_size);
        float emit        = light_info.r;

        if (emit >= 1.0) {
          discard;
        }

        float gloss       = light_info.g;
        vec4 light        = texture(light_tex, texcoords);
        vec3 light_dir    = normalize(light.rgb - 0.5);
        float attenuation = light.a;

        fragLight = get_light(
          gl_FragCoord.xy/screen_size, light_dir,
          light_color[instance_id], gloss, emit, attenuation);
      }
    )"
  )
  , projection(get_uniform<math::mat3>("projection"))
  , transform(get_uniform<math::mat3>("transform"))
  , depth(get_uniform<float>("depth"))
  , parallax(get_uniform<float>("parallax"))
  , screen_size(get_uniform<math::vec2i>("screen_size"))
  , g_buffer_normal(get_uniform<int>("g_buffer_normal"))
  , g_buffer_light(get_uniform<int>("g_buffer_light"))
  , light_tex(get_uniform<int>("light_tex"))
  , light_color(get_uniform<math::vec4>("light_color")) {}

////////////////////////////////////////////////////////////////////////////////

}
