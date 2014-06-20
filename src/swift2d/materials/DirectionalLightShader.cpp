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
    // vertex shader
    R"(
      @include "quad_vertext_shader"
    )",
    // fragment shader
    R"(
      @include "version"

      // varyings
      in vec2 tex_coords;

      // uniforms
      uniform ivec2     screen_size;
      uniform vec3      light_dir;
      uniform sampler2D g_buffer_normal;

      // output
      out vec3 fragColor;

      void main(void){
        vec4 normal       = texture2D(g_buffer_normal, gl_FragCoord.xy/screen_size);
        vec3 surface_dir  = normalize(normal.rgb - 0.5);
        float attenuation = normal.a;

        float spot        = pow(max(0, dot(vec3(0, 0, 1), reflect(light_dir, surface_dir))), 50) * attenuation;
        float intensity   = max(0, dot(light_dir, surface_dir)) * attenuation;

        fragColor         = vec3(intensity, spot, 0);
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
