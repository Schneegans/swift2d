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

      layout(location=0) in int   type;
      layout(location=1) in vec2  position;
      layout(location=2) in vec2  velocity;
      layout(location=3) in float life;

      out int   varying_type;
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

      in int   varying_type[];
      in vec2  varying_position[];
      in vec2  varying_velocity[];
      in float varying_life[];

      out int   out_type;
      out vec2  out_position;
      out vec2  out_velocity;
      out float out_life;

      void main(void) {

        if (varying_type[0] == 1) {
          // launch new particle -----------------------------------------------

          // launch particle
          out_type = 2;
          out_position = varying_position[0];
          out_life = 10;
          out_velocity = varying_velocity[0];
          EmitVertex();
          EndPrimitive();

          // copy launcher
          out_type = 1;
          out_position = varying_position[0];
          out_life = varying_life[0];
          out_velocity = varying_velocity[0];
          EmitVertex();
          EndPrimitive();

        } else {
          // update existing particles -----------------------------------------
          if (varying_life[0] > 0.01) {
            out_type = 2;
            out_position = varying_position[0] + varying_velocity[0];
            out_life = varying_life[0] - 0.01;
            out_velocity = varying_velocity[0];
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
