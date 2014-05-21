////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/LightShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LightShader::LightShader()
  : Shader(
    // vertex shader -----------------------------------------------------------
    R"(
      #version 330

      // input
      layout(location=0) in vec2 position;

      // uniforms
      uniform mat3 projection;
      uniform mat3 transform;

      // varyings
      out vec2 tex_coords;

      void main(void) {
        vec3 pos    = projection * transform * vec3(position, 1.0);
        tex_coords  = position*0.5 + 0.5;
        gl_Position = vec4(pos.xy, 0.0, 1.0);
      }
    )",
    // fragment shader ---------------------------------------------------------
    R"(
      #version 330

      in vec2 tex_coords;

      uniform ivec2 screen_size;
      uniform sampler2D light_tex;
      uniform sampler2D g_buffer_normal;

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
