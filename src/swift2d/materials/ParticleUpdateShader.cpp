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
      layout(location=3) in float life;

      out float varying_type;
      out vec2  varying_position;
      out vec2  varying_velocity;
      out float varying_life;

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
      in float varying_life[];

      uniform sampler2D gravity_map;
      uniform mat3      projection;
      uniform mat3      transform;
      uniform vec2      time;

      out float out_type;
      out vec2  out_position;
      out vec2  out_velocity;
      out float out_life;

      void main(void) {

        if (varying_type[0] == 1) {

          // launch new particle -----------------------------------------------
          if (varying_life[0] > 0) {

            // copy launcher
            out_type = 1;
            out_position = vec2(0, 0);
            out_life = varying_life[0] - time.x;
            out_velocity = vec2(0, 0);
            EmitVertex();
            EndPrimitive();

          } else {

            // copy launcher
            out_type = 1;
            out_position = vec2(0, 0);
            out_life = 0.0;
            out_velocity = vec2(0, 0);
            EmitVertex();
            EndPrimitive();

            // launch particle
            out_type = 2;
            out_position = (transform * vec3(0, 0, 1)).xy;
            out_life = 150000;
            out_velocity = vec2(0, 0);
            EmitVertex();
            EndPrimitive();
          }

        } else {

          // update existing particles -----------------------------------------
          if (varying_life[0] > 0) {

            vec2 texcoords = ((projection * vec3(varying_position[0], 1)).xy + 1.0) * 0.5;
            vec2 gravity = (texture2D(gravity_map, texcoords).rg - 0.5);

            out_type = 2;
            out_position = varying_position[0] + varying_velocity[0] * time.x / 1000;
            out_life = varying_life[0] - time.x;
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
