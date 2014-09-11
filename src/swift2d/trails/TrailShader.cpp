////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/trails/TrailShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TrailShader::TrailShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2  position;
      layout(location=1) in vec2  life;
      layout(location=2) in vec2  prev_1_position;
      layout(location=3) in vec2  prev_2_position;
      layout(location=4) in vec2  prev_3_position;

      out float varying_age;
      out vec2  varying_prev_1_position;
      out vec2  varying_prev_2_position;
      out vec2  varying_prev_3_position;

      void main(void) {
        gl_Position = vec4(position, 0.0, 1.0);

        varying_prev_1_position = prev_1_position;
        varying_prev_2_position = prev_2_position;
        varying_prev_3_position = prev_3_position;

        varying_age             = life.x;
      }
    )",

    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      in float age;

      uniform vec4 start_color;
      uniform vec4 end_color;
      uniform vec2 glow;

      @include "write_gbuffer"

      void main(void) {
        vec4  c = mix(start_color, end_color, age);
        float g = mix(glow.x,      glow.y,    age);

        write_gbuffer(c, g);
      }
    )",


    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 404) out;

      in float varying_age[];
      in vec2 varying_prev_1_position[];
      in vec2 varying_prev_2_position[];
      in vec2 varying_prev_3_position[];

      uniform mat3 projection;
      uniform float width;

      uniform int  emitter_count;
      uniform vec2 curr_emitter_positions[100];
      uniform vec4 prev_emitter_positions[100];

      out float age;
      out float vertex_id;

      float flip_ccw(vec2 v1, vec2 v2) {
        return v1.x * v2.y - v2.x * v1.y > 0.0 ? 1.0 : -1.0;
      }

      void main(void) {
        float r = width * 0.5;

        vec2 p2_to_p3 = normalize(varying_prev_3_position[0] - varying_prev_2_position[0]);
        vec2 p2_to_p1 = normalize(varying_prev_1_position[0] - varying_prev_2_position[0]);

        vec2 n1 = normalize(p2_to_p3 + p2_to_p1) * flip_ccw(p2_to_p1, p2_to_p3);

        vec3 v1 = projection * vec3(varying_prev_2_position[0] + r * n1, 1.0);
        vec3 v2 = projection * vec3(varying_prev_2_position[0] - r * n1, 1.0);

        vec2 p1_to_p2 = normalize(varying_prev_2_position[0] - varying_prev_1_position[0]);
        vec2 p1_to_p0 = normalize(gl_in[0].gl_Position.xy - varying_prev_1_position[0]);

        vec2 n2 = normalize(p1_to_p2 + p1_to_p0) * flip_ccw(p1_to_p0, p1_to_p2);

        vec3 v3 = projection * vec3(varying_prev_1_position[0] + r * n2, 1.0);
        vec3 v4 = projection * vec3(varying_prev_1_position[0] - r * n2, 1.0);


        gl_Position = vec4(v1, 1.0);
        age = varying_age[0];
        EmitVertex();

        gl_Position = vec4(v2, 1.0);
        age = varying_age[0];
        EmitVertex();

        gl_Position = vec4(v3, 1.0);
        age = varying_age[0];
        EmitVertex();

        gl_Position = vec4(v4, 1.0);
        age = varying_age[0];
        EmitVertex();

        EndPrimitive();
      }
    )"
  )
  , projection(get_uniform<math::mat3>("projection"))
  , width(get_uniform<float>("width"))
  , start_color(get_uniform<math::vec4>("start_color"))
  , end_color(get_uniform<math::vec4>("end_color"))
  , glow(get_uniform<math::vec2>("glow")) {}

////////////////////////////////////////////////////////////////////////////////

}
