////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/trails/TrailShaderBase.hpp>
#include <swift2d/materials/ShaderIncludes.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TrailShaderBase::TrailShaderBase() {

  ShaderIncludes::get().add_include("trail_vertex_shader",
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2  position;
      layout(location=1) in vec2  life;
      layout(location=2) in vec2  prev_u_texcoords;
      layout(location=3) in vec2  prev_1_position;
      layout(location=4) in vec2  prev_2_position;
      layout(location=5) in vec2  prev_3_position;

      out float varying_age;
      out vec2  varying_prev_u_texcoods;
      out vec2  varying_prev_1_position;
      out vec2  varying_prev_2_position;
      out vec2  varying_prev_3_position;

      void main(void) {
        gl_Position = vec4(position, 0.0, 1.0);

        varying_prev_1_position = prev_1_position;
        varying_prev_2_position = prev_2_position;
        varying_prev_3_position = prev_3_position;
        varying_prev_u_texcoods = prev_u_texcoords;

        varying_age             = life.x;
      }
    )");

  ShaderIncludes::get().add_include("trail_fragment_shader",
    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in float varying_age[];
      in vec2 varying_prev_1_position[];
      in vec2 varying_prev_2_position[];
      in vec2 varying_prev_3_position[];
      in vec2 varying_prev_u_texcoods[];


      @include "camera_uniforms"
      uniform float start_width;
      uniform float end_width;

      out vec2 texcoords;

      float flip_ccw(vec2 v1, vec2 v2) {
        return v1.x * v2.y - v2.x * v1.y > 0.0 ? 1.0 : -1.0;
      }

      void main(void) {
        float current_age = varying_prev_u_texcoods[0].y;
        float r = mix(start_width * 0.5, end_width * 0.5, current_age);

        vec2 p2_to_p3 = normalize(varying_prev_3_position[0] - varying_prev_2_position[0]);
        vec2 p2_to_p1 = normalize(varying_prev_1_position[0] - varying_prev_2_position[0]);

        vec2 n1 = normalize(p2_to_p3 + p2_to_p1) * flip_ccw(p2_to_p1, p2_to_p3);

        vec3 v1 = projection * vec3(varying_prev_2_position[0] + r * n1, 1.0);
        vec3 v2 = projection * vec3(varying_prev_2_position[0] - r * n1, 1.0);

        gl_Position = vec4(v1, 1.0);
        texcoords = vec2(current_age, 1.0);
        EmitVertex();

        gl_Position = vec4(v2, 1.0);
        texcoords = vec2(current_age, 0.0);
        EmitVertex();


        current_age = varying_prev_u_texcoods[0].x;
        r = mix(start_width * 0.5, end_width * 0.5, current_age);

        vec2 p1_to_p2 = normalize(varying_prev_2_position[0] - varying_prev_1_position[0]);
        vec2 p1_to_p0 = normalize(gl_in[0].gl_Position.xy - varying_prev_1_position[0]);

        vec2 n2 = normalize(p1_to_p2 + p1_to_p0) * flip_ccw(p1_to_p0, p1_to_p2);

        vec3 v3 = projection * vec3(varying_prev_1_position[0] + r * n2, 1.0);
        vec3 v4 = projection * vec3(varying_prev_1_position[0] - r * n2, 1.0);

        gl_Position = vec4(v3, 1.0);
        texcoords = vec2(current_age, 1.0);
        EmitVertex();

        gl_Position = vec4(v4, 1.0);
        texcoords = vec2(current_age, 0.0);
        EmitVertex();

        EndPrimitive();
      }
    )"
  );
}
}

