////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/trails/TrailUpdateShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TrailUpdateShader::TrailUpdateShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2  position;
      layout(location=1) in vec2  life;
      layout(location=2) in vec2  prev_1_position;
      layout(location=3) in vec2  prev_2_position;
      layout(location=4) in vec2  prev_3_position;

      out vec2 varying_position;
      out vec2 varying_life;
      out vec2 varying_prev_1_position;
      out vec2 varying_prev_2_position;
      out vec2 varying_prev_3_position;

      void main(void) {
        varying_position = position;
        varying_life = life;
        varying_prev_1_position = prev_1_position;
        varying_prev_2_position = prev_2_position;
        varying_prev_3_position = prev_3_position;
      }
    )",
    "", // empty fragment shader
    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(points) out;
      layout(max_vertices = 30) out;

      in vec2  varying_position[];
      in vec2  varying_life[];
      in vec2  varying_prev_1_position[];
      in vec2  varying_prev_2_position[];
      in vec2  varying_prev_3_position[];

      // general uniforms
      uniform vec2      time;         // x: frame time  y: total time [millisec]

      // spawn uniforms
      uniform int       spawn_count;
      uniform mat3      transform;
      uniform mat3      prev_1_transform;
      uniform mat3      prev_2_transform;
      uniform mat3      prev_3_transform;
      uniform float     life;         //  [sec]


      out vec2 out_position;
      out vec2 out_life;
      out vec2 out_prev_1_position;
      out vec2 out_prev_2_position;
      out vec2 out_prev_3_position;

      void main(void) {

        if (spawn_count >= 0) {

          // spawn new trail points -----------------------------------------------

          for (int i=0; i<spawn_count; ++i) {

            out_position = (transform * vec3(0.0, 0.0, 1)).xy;
            out_prev_1_position = (prev_1_transform * vec3(0.0, 0.0, 1)).xy;
            out_prev_2_position = (prev_2_transform * vec3(0.0, 0.0, 1)).xy;
            out_prev_3_position = (prev_3_transform * vec3(0.0, 0.0, 1)).xy;
            out_life     = vec2(0, life*1000.0);

            EmitVertex(); EndPrimitive();
          }

        } else {

          // update existing trail points -----------------------------------------

          if (varying_life[0].x < 1) {

            out_position = varying_position[0];
            out_prev_1_position = varying_prev_1_position[0];
            out_prev_2_position = varying_prev_2_position[0];
            out_prev_3_position = varying_prev_3_position[0];
            out_life     = vec2(varying_life[0].x + time.x/varying_life[0].y, varying_life[0].y);

            EmitVertex(); EndPrimitive();
          }
        }
      }
    )",
    {"out_position", "out_life", "out_prev_1_position", "out_prev_2_position", "out_prev_3_position"}
  )
  , time(get_uniform<math::vec2>("time"))
  , spawn_count(get_uniform<int>("spawn_count"))
  , transform(get_uniform<math::mat3>("transform"))
  , prev_1_transform(get_uniform<math::mat3>("prev_1_transform"))
  , prev_2_transform(get_uniform<math::mat3>("prev_2_transform"))
  , prev_3_transform(get_uniform<math::mat3>("prev_3_transform"))
  , life(get_uniform<float>("life")) {}

////////////////////////////////////////////////////////////////////////////////

}
