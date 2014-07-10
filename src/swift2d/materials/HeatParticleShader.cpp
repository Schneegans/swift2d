////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/HeatParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HeatParticleShader::HeatParticleShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2  position;
      layout(location=1) in float age;

      out float varying_age;

      void main(void) {
        gl_Position = vec4(position.xy, 0.0, 1.0);
        varying_age = age;
      }
    )",


    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      in float age;
      in vec2  tex_coords;

      // uniforms
      uniform float start_opacity;
      uniform float end_opacity;

      uniform sampler2D heat_tex;

      layout (location = 0) out vec4 fragColor;

      void main(void) {
        float opacity = mix(start_opacity, end_opacity, age);
        fragColor = texture2D(heat_tex, tex_coords);
        fragColor.a *= opacity;
      }
    )",

    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

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
  )
  , projection(get_uniform<math::mat3>("projection"))
  , transform(get_uniform<math::mat3>("transform"))
  , heat_tex(get_uniform<int>("heat_tex"))
  , start_scale(get_uniform<float>("start_scale"))
  , end_scale(get_uniform<float>("end_scale"))
  , start_opacity(get_uniform<float>("start_opacity"))
  , end_opacity(get_uniform<float>("end_opacity")) {}

////////////////////////////////////////////////////////////////////////////////

}
