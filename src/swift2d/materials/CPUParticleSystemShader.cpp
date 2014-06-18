////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/CPUParticleSystemShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

CPUParticleSystemShader::CPUParticleSystemShader()
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

      // in  float geomAge;
      layout (location = 0) out vec4 fragColor;
      layout (location = 1) out vec4 fragNormal;
      layout (location = 2) out vec4 fragEmit;

      in float age;
      in vec2  tex_coords;

      uniform sampler2D diffuse;

      void main(void) {
        fragColor    = texture2D(diffuse, tex_coords);
        fragColor.a *= (1.0 - age);
        fragNormal   = vec4(0.5, 0.5, 0, fragColor.a);
        fragEmit     = vec4(1.0, 0, 0, fragColor.a);
      }
    )",


    R"(
      #version 330

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      uniform mat3 transform;
      uniform mat3 projection;

      in  float varying_age[];
      out float age;
      out vec2  tex_coords;

      void main(void) {

        if (varying_age[0] >= 0) {
          float yo[2] = float[2](0.5, -0.5);
          float xo[2] = float[2](0.5, -0.5);

          for(int j=0; j!=2; ++j) {
            for(int i=0; i!=2; ++i) {
              vec2 in_pos = gl_in[0].gl_Position.xy - vec2(xo[i], yo[j]);
              vec3 pos    = projection * transform * vec3(in_pos, 1.0);
              gl_Position = vec4(pos.xy, 0.0, 1.0);
              age         = varying_age[0];
              tex_coords  = vec2(xo[i], yo[j]) + 0.5;
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
