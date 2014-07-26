////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/SpriteParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteParticleShader::SpriteParticleShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2 position;
      layout(location=1) in vec2 velocity;
      layout(location=2) in vec2 life;

      out float varying_age;

      void main(void) {
        gl_Position = vec4(position, 0.0, 1.0);

        varying_age = life.x;
      }
    )",


    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      in float age;
      in vec2  tex_coords;

      uniform sampler2D diffuse;

      @include "write_gbuffer"

      void main(void) {
        // vec4 color = mix(
        //   vec4(start_color, start_opacity),
        //   vec4(end_color, end_opacity),
        //   age
        // );

        // float glow = mix(
        //   start_glow,
        //   end_glow,
        //   age
        // );

        // write_gbuffer(vec4(1, 0, 0, 0.5), 0);
        // write_gbuffer(texture2D(diffuse, tex_coords) * color, glow);
        write_gbuffer(texture2D(diffuse, tex_coords), 0);
      }
    )",


    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in float varying_age[];

      // uniform mat3  transform;
      uniform mat3  projection;
      uniform vec2  scale;
      // uniform float end_scale;
      // uniform bool  enable_rotation;

      out float age;
      out vec2  tex_coords;

      void main(void) {

        // if (varying_age[0] >= 0) {
          float yo[2] = float[2](0.5, -0.5);
          float xo[2] = float[2](0.5, -0.5);

          float scale = mix(scale.x, scale.y, varying_age[0]);

          for(int j=0; j!=2; ++j) {
            for(int i=0; i!=2; ++i) {

              vec2 pos = vec2(xo[i], yo[j]) * scale;

              // if (enable_rotation) {
              //   float r = varying_rotation[0];
              //   pos = vec2(
              //     pos.x * cos(r) - pos.y * sin(r),
              //     pos.y * cos(r) + pos.x * sin(r)
              //   );
              // }

              pos = gl_in[0].gl_Position.xy - pos;
              pos = (projection * vec3(pos, 1.0)).xy;
              // pos = (projection * transform * vec3(pos, 1.0)).xy;

              gl_Position = vec4(pos, 0.0, 1.0);
              age         = varying_age[0];
              tex_coords  = vec2(xo[i], yo[j]) + 0.5;
              EmitVertex();
            }
          }
          EndPrimitive();
        // }
      }
    )",
    {}
  ) {}
  // , projection(get_uniform<math::mat3>("projection"))
  // , transform(get_uniform<math::mat3>("transform"))
  // , diffuse(get_uniform<int>("diffuse"))
  // , start_scale(get_uniform<float>("start_scale"))
  // , end_scale(get_uniform<float>("end_scale"))
  // , start_color(get_uniform<math::vec3>("start_color"))
  // , end_color(get_uniform<math::vec3>("end_color"))
  // , start_opacity(get_uniform<float>("start_opacity"))
  // , end_opacity(get_uniform<float>("end_opacity"))
  // , start_glow(get_uniform<float>("start_glow"))
  // , end_glow(get_uniform<float>("end_glow"))
  // , enable_rotation(get_uniform<int>("enable_rotation")) {}

////////////////////////////////////////////////////////////////////////////////

}