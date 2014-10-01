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

      layout(location=0) in vec2   position;
      layout(location=1) in vec2   life;
      layout(location=2) in vec2   prev_u_texcoords;
      layout(location=3) in vec2   prev_1_position;
      layout(location=4) in vec2   prev_2_position;
      layout(location=5) in vec2   prev_3_position;

      out vec2  varying_position;
      out vec2  varying_life;
      out vec2  varying_prev_u_texcoords;
      out vec2  varying_prev_1_position;
      out vec2  varying_prev_2_position;
      out vec2  varying_prev_3_position;

      void main(void) {
        varying_position = position;
        varying_life = life;
        varying_prev_u_texcoords = prev_u_texcoords;
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
      layout(max_vertices = 50) out;

      in vec2  varying_position[];
      in vec2  varying_life[];
      in vec2  varying_prev_u_texcoords[];
      in vec2  varying_prev_1_position[];
      in vec2  varying_prev_2_position[];
      in vec2  varying_prev_3_position[];

      // general uniforms
      uniform vec2      time;         // x: frame time  y: total time [millisec]

      // spawn uniforms
      uniform vec2  position[50];
      uniform vec2  prev_1_position[50];
      uniform vec2  prev_2_position[50];
      uniform vec2  prev_3_position[50];
      uniform vec2  time_since_prev_spawns[50]; // x: last  y: prev_1

      uniform int   spawn_count;
      uniform float life;         //  [millisec]
      uniform int   use_global_texcoords;


      out vec2  out_position;
      out vec2  out_life;
      out vec2  out_prev_u_texcoords;
      out vec2  out_prev_1_position;
      out vec2  out_prev_2_position;
      out vec2  out_prev_3_position;

      void main(void) {

        if (spawn_count >= 0) {


          // spawn new trail points -----------------------------------------------

          for (int i=0; i<spawn_count; ++i) {
            vec2 texcoords = (-time_since_prev_spawns[i] + time.y) / life;

            if (use_global_texcoords == 0) {
              texcoords = time_since_prev_spawns[i]/life;
            }

            out_position = position[i];
            out_prev_1_position = prev_1_position[i];
            out_prev_2_position = prev_2_position[i];
            out_prev_3_position = prev_3_position[i];
            out_life     = vec2(0, life);
            out_prev_u_texcoords = texcoords;

            EmitVertex(); EndPrimitive();
          }


        } else {

          // update existing trail points -----------------------------------------

          if (varying_life[0].x < 1) {

            vec2 texcoords = varying_prev_u_texcoords[0];

            if (use_global_texcoords == 0) {
              texcoords = texcoords + time.x/varying_life[0].y;
            }

            out_position = varying_position[0];
            out_prev_1_position = varying_prev_1_position[0];
            out_prev_2_position = varying_prev_2_position[0];
            out_prev_3_position = varying_prev_3_position[0];
            out_life     = vec2(varying_life[0].x + time.x/varying_life[0].y, varying_life[0].y);
            out_prev_u_texcoords = texcoords;

            EmitVertex(); EndPrimitive();
          }
        }
      }
    )",
    {
     "out_position", "out_life", "out_prev_u_texcoords",
     "out_prev_1_position", "out_prev_2_position", "out_prev_3_position"
    }
  )
  , time(get_uniform<math::vec2>("time"))
  , time_since_prev_spawns(get_uniform<math::vec2>("time_since_prev_spawns"))
  , spawn_count(get_uniform<int>("spawn_count"))
  , position(get_uniform<math::vec2>("position"))
  , prev_1_position(get_uniform<math::vec2>("prev_1_position"))
  , prev_2_position(get_uniform<math::vec2>("prev_2_position"))
  , prev_3_position(get_uniform<math::vec2>("prev_3_position"))
  , life(get_uniform<float>("life"))
  , use_global_texcoords(get_uniform<int>("use_global_texcoords"))
  {}

////////////////////////////////////////////////////////////////////////////////

}
