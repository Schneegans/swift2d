////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/LightShader.hpp>
#include <swift2d/materials/shader_snippets.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LightShader::LightShader()
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
        vec4 normal      = texture2D(g_buffer_normal, gl_FragCoord.xy/screen_size);
        vec3 light       = texture2D(light_tex, tex_coords).rgb;

        vec3 light_dir   = normalize(light.rgb - 0.5);
        vec3 surface_dir = normalize(normal.rgb - 0.5);

        float spot       = pow(max(0, dot(vec3(0, 0, -1), reflect(-light_dir, surface_dir))), 50) * normal.a;
        float intensity  = max(0, dot(light_dir, surface_dir)) * normal.a;

        fragColor        = vec3(min(1, intensity*0.5 + spot));
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
