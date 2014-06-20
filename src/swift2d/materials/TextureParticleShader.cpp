////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/TextureParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TextureParticleShader::TextureParticleShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2  position;
      layout(location=1) in float age;
      layout(location=2) in float rotation;

      out float varying_age;
      out float varying_rotation;

      void main(void) {
        gl_Position = vec4(position.xy, 0.0, 1.0);
        varying_age = age;
        varying_rotation = rotation;
      }
    )",


    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      in float age;
      in vec2  tex_coords;

      uniform sampler2D diffuse;
      uniform vec3      start_color;
      uniform vec3      end_color;
      uniform float     start_opacity;
      uniform float     end_opacity;

      @include "write_gbuffer"

      void main(void) {
        vec4 color = mix(
          vec4(start_color, start_opacity),
          vec4(end_color, end_opacity),
          age
        );

        vec4 out_color  = texture2D(diffuse, tex_coords) * color;
        vec4 out_normal = vec4(0.5, 0.5, 0, out_color.a);
        vec4 out_emit   = vec4(1.0, 0, 0, out_color.a);

        write_gbuffer(out_color, out_normal, out_emit);
      }
    )",


    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in float varying_rotation[];
      in float varying_age[];

      uniform mat3  transform;
      uniform mat3  projection;
      uniform float start_scale;
      uniform float end_scale;
      uniform bool  enable_rotation;

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

              if (enable_rotation) {
                float r = varying_rotation[0];
                pos = vec2(
                  pos.x * cos(r) - pos.y * sin(r),
                  pos.y * cos(r) + pos.x * sin(r)
                );
              }

              pos = gl_in[0].gl_Position.xy - pos;
              pos = (projection * transform * vec3(pos, 1.0)).xy;

              gl_Position = vec4(pos, 0.0, 1.0);
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
