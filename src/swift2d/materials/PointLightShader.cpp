////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/PointLightShader.hpp>
#include <swift2d/materials/shader_snippets.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointLightShader::PointLightShader()
  : Shader(
    // vertex shader
    shader_snippets::get_quad_vertext_shader(),
    // fragment shader
    R"(
      #version 330

      // varyings
      in vec2 tex_coords;

      // uniforms
      uniform ivec2 screen_size;
      uniform sampler2D light_tex;
      uniform sampler2D g_buffer_normal;

      // output
      out vec3 fragColor;

      void main(void){
        vec4 normal       = texture2D(g_buffer_normal, gl_FragCoord.xy/screen_size);
        vec4 light        = texture2D(light_tex, tex_coords);

        vec3 light_dir    = normalize(light.rgb - 0.5);
        vec3 surface_dir  = normalize(normal.rgb - 0.5);
        float attenuation = light.a * normal.a;

        float spot        = pow(max(0, dot(vec3(0, 0, 1), reflect(light_dir, surface_dir))), 50) * attenuation;
        float intensity   = max(0, dot(light_dir, surface_dir)) * attenuation;

        fragColor         = vec3(intensity, spot, 0);
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}