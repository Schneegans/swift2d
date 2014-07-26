////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/PointParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointParticleShader::PointParticleShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2 position;
      layout(location=1) in vec2 velocity;
      layout(location=2) in vec2 life;

      uniform mat3 projection;

      out float age;

      void main(void) {
        vec2 pos = (projection * vec3(position, 1.0)).xy;
        gl_Position = vec4(pos, 0.0, 1.0);

        age = life.x;
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
        float g = mix(glow.x, glow.y, age);

        write_gbuffer(c, g);
      }
    )"
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
