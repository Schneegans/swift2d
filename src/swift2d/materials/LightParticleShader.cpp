////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/LightParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LightParticleShader::LightParticleShader()
  : Shader(
    R"(
      #version 330

      layout(location=0) in vec2  position;
      layout(location=1) in float age;

      out float varying_age;

      void main(void) {
        gl_Position = vec4(position.xy, 0.0, 1.0);
        varying_age = age;
      }
    )",


    R"(
      #version 330

      in float age;
      in vec2  tex_coords;

      // uniforms
      uniform sampler2D light_tex;
      uniform vec3      start_color;
      uniform vec3      end_color;
      uniform float     start_opacity;
      uniform float     end_opacity;

      uniform ivec2 screen_size;
      uniform sampler2D g_buffer_normal;

      // output
      out vec3 fragColor;

      void main(void) {
        vec4 color = mix(
          vec4(start_color, start_opacity),
          vec4(end_color, end_opacity),
          age
        );

        vec4 normal       = texture2D(g_buffer_normal, gl_FragCoord.xy/screen_size);
        vec4 light        = texture2D(light_tex, tex_coords);

        vec3 light_dir    = normalize(light.rgb - 0.5);
        vec3 surface_dir  = normalize(normal.rgb - 0.5);
        float attenuation = light.a * normal.a * color.a;

        float spot        = pow(max(0, dot(vec3(0, 0, 1), reflect(light_dir, surface_dir))), 50) * attenuation;
        float intensity   = max(0, dot(light_dir, surface_dir)) * attenuation;

        fragColor         = vec3(intensity, spot, 0) + color.rga*0.00001;
      }
    )",


    R"(
      #version 330

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in float varying_age[];

      uniform mat3  transform;
      uniform mat3  projection;
      uniform float start_scale;
      uniform float end_scale;

      out float age;
      out vec2  tex_coords;

      void main(void) {

        if (varying_age[0] >= 0) {
          float yo[2] = float[2](0.5, -0.5);
          float xo[2] = float[2](0.5, -0.5);

          float scale = mix(start_scale, end_scale, varying_age[0]);

          for(int j=0; j!=2; ++j) {
            for(int i=0; i!=2; ++i) {

              vec2 pos = vec2(xo[i], yo[j]) * scale;

              pos = gl_in[0].gl_Position.xy - pos;
              pos = (projection * transform * vec3(pos, 1.0)).xy;

              gl_Position = vec4(pos, 0.0, 1.0);
              age         = varying_age[0];
              tex_coords  = vec2(-xo[i], yo[j]) + 0.5;
              EmitVertex();
            }
          }
          EndPrimitive();
        }
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
