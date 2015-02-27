////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
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

      out vec2  varying_life;
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

        varying_life             = life;
      }
    )");

  ShaderIncludes::get().add_include("trail_tf_shader",
    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in vec2 varying_life[];
      in vec2 varying_prev_1_position[];
      in vec2 varying_prev_2_position[];
      in vec2 varying_prev_3_position[];
      in vec2 varying_prev_u_texcoods[];


      @include "camera_uniforms"
      uniform float start_width;
      uniform float end_width;
      uniform float total_time;
      uniform int use_global_texcoords;

      out vec2  texcoords;
      out float age;

      float flip_ccw(vec2 v1, vec2 v2) {
        return v1.x * v2.y - v2.x * v1.y > 0.0 ? 1.0 : -1.0;
      }

      void main(void) {
        age = varying_prev_u_texcoods[0].y;

        if (use_global_texcoords == 1) {
          age = total_time / varying_life[0].y - age;
        }

        float r = mix(start_width * 0.5, end_width * 0.5, age);

        vec2 p3_to_p2 = normalize(varying_prev_2_position[0] - varying_prev_3_position[0]);
        vec2 p2_to_p1 = normalize(varying_prev_1_position[0] - varying_prev_2_position[0]);
        vec2 p1_to_p0 = normalize(gl_in[0].gl_Position.xy - varying_prev_1_position[0]);

        vec2 n1 = vec2(0.5, -0.5)*(p3_to_p2.yx + p2_to_p1.yx)*r;

        vec3 v1 = projection * vec3(varying_prev_2_position[0] + n1, 1.0);
        vec3 v2 = projection * vec3(varying_prev_2_position[0] - n1, 1.0);

        gl_Position = vec4(v1, 1.0);
        texcoords = vec2(varying_prev_u_texcoods[0].y, 1.0);
        EmitVertex();

        gl_Position = vec4(v2, 1.0);
        texcoords = vec2(varying_prev_u_texcoods[0].y, 0.0);
        EmitVertex();


        age = varying_prev_u_texcoods[0].x;

        if (use_global_texcoords == 1) {
          age = total_time / varying_life[0].y - age;
        }

        r = mix(start_width * 0.5, end_width * 0.5, age);

        vec2 n2 = vec2(0.5, -0.5)*(p2_to_p1.yx + p1_to_p0.yx)*r;

        vec3 v3 = projection * vec3(varying_prev_1_position[0] + n2, 1.0);
        vec3 v4 = projection * vec3(varying_prev_1_position[0] - n2, 1.0);

        gl_Position = vec4(v3, 1.0);
        texcoords = vec2(varying_prev_u_texcoods[0].x, 1.0);
        EmitVertex();

        gl_Position = vec4(v4, 1.0);
        texcoords = vec2(varying_prev_u_texcoods[0].x, 0.0);
        EmitVertex();

        EndPrimitive();
      }
    )"
  );
}
}

