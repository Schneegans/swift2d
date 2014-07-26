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
      uniform vec4      start_color;
      uniform vec4      end_color;
      uniform vec2      glow;

      @include "write_gbuffer"

      void main(void) {
        vec4  c = mix(start_color, end_color, age);
        float g = mix(glow.x,      glow.y,    age);

        write_gbuffer(texture2D(diffuse, tex_coords) * c, g);
      }
    )",


    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in float varying_age[];

      uniform mat3  projection;
      uniform vec2  scale;
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
  )
  , projection(get_uniform<math::mat3>("projection"))
  , diffuse(get_uniform<int>("diffuse"))
  , scale(get_uniform<math::vec2>("scale"))
  , start_color(get_uniform<math::vec4>("start_color"))
  , end_color(get_uniform<math::vec4>("end_color"))
  , glow(get_uniform<math::vec2>("glow")) {}
  // , enable_rotation(get_uniform<int>("enable_rotation")) {}

////////////////////////////////////////////////////////////////////////////////

}
