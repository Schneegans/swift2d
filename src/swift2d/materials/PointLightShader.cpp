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
      @include "get_lit_surface_color"

      layout (location = 0) out vec3 fragColor;

      void main(void){
        vec4 light        = texture2D(light_tex, texcoords);
        vec3 light_dir    = normalize(light.rgb - 0.5);
        float attenuation = light.a;

        fragColor = get_lit_surface_color(gl_FragCoord.xy/screen_size, light_dir, light_color[instance_id], attenuation);
      }
    )"
  )
  , projection(get_uniform<math::mat3>("projection"))
  , transform(get_uniform<math::mat3>("transform"))
  , depth(get_uniform<float>("depth"))
  , parallax(get_uniform<float>("parallax"))
  , screen_size(get_uniform<math::vec2i>("screen_size"))
  , g_buffer_diffuse(get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_normal(get_uniform<int>("g_buffer_normal"))
  , g_buffer_light(get_uniform<int>("g_buffer_light"))
  , light_tex(get_uniform<int>("light_tex"))
  , light_color(get_uniform<math::vec4>("light_color")) {}

////////////////////////////////////////////////////////////////////////////////

}
