////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/ParticleUpdateShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleUpdateShader::ParticleUpdateShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in float type;
      layout(location=1) in vec2  position;
      layout(location=2) in vec2  velocity;
      layout(location=3) in vec2  life;

      out float varying_type;
      out vec2  varying_position;
      out vec2  varying_velocity;
      out vec2  varying_life;

      void main(void) {
        varying_type = type;
        varying_position = position;
        varying_velocity = velocity;
        varying_life = life;
      }
    )",
    "", // empty fragment shader
    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(points) out;
      layout(max_vertices = 30) out;

      in float varying_type[];
      in vec2  varying_position[];
      in vec2  varying_velocity[];
      in vec2  varying_life[];

      uniform sampler2D gravity_map;
      uniform mat3      projection;
      uniform mat3      transform;
      uniform vec2      time;
      uniform float     mass;
      uniform int       spawn_count;

      out float out_type;
      out vec2  out_position;
      out vec2  out_velocity;
      out vec2  out_life;

      void main(void) {

        if (varying_type[0] == 1) {

          // launch new particles ----------------------------------------------

          // copy launcher
          out_type = 1;
          out_position = vec2(0, 0);
          out_life     = vec2(0, 0);
          out_velocity = vec2(0, 0);
          EmitVertex();
          EndPrimitive();

          for (int i=0; i<spawn_count; ++i) {
            out_type = 2;
            out_position = (transform * vec3(0, 0, 1)).xy;
            out_life     = vec2(0, 1000);
            out_velocity = vec2(0, 0);
            EmitVertex();
            EndPrimitive();
          }

        } else {

          // update existing particles -----------------------------------------
          if (varying_life[0].x < 1) {

            vec2 texcoords = ((projection * vec3(varying_position[0], 1)).xy + 1.0) * 0.5;
            vec2 gravity = (texture2D(gravity_map, texcoords).rg - 0.5) * mass;

            out_type = 2;
            out_position = varying_position[0] + varying_velocity[0] * time.x / 1000;
            out_life     = vec2(varying_life[0].x + time.x/varying_life[0].y, varying_life[0].y);
            out_velocity = (varying_velocity[0] + gravity)*0.9;
            EmitVertex();
            EndPrimitive();
          }
        }
      }
    )",
    {"out_type", "out_position", "out_velocity", "out_life"}
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
