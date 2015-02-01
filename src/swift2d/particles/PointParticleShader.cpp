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

      @include "camera_uniforms"

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
      uniform vec4 mid_color;
      uniform vec4 end_color;
      uniform vec4 glow_mid_life;
      uniform vec3 burn;

      @include "write_gbuffer"
      @include "three_way_mix"

      void main(void) {
        vec4  c = three_way_mix(start_color, mid_color, end_color, glow_mid_life.w, age);
        float g = three_way_mix(glow_mid_life.x, glow_mid_life.y, glow_mid_life.z, glow_mid_life.w, age);
        float b = three_way_mix(burn.x, burn.y, burn.z, glow_mid_life.w, age);

        float alpha = c.a;
        vec3 color = c.rgb * alpha;
        alpha = alpha*(1-b);
        float emit = 1.0 * alpha;
        float glow = g * alpha;

        write_gbuffer_premultiplied(vec4(color, alpha), emit, glow);
      }
    )"
  )
  , projection(get_uniform<math::mat3>("projection"))
  , start_color(get_uniform<math::vec4>("start_color"))
  , mid_color(get_uniform<math::vec4>("mid_color"))
  , end_color(get_uniform<math::vec4>("end_color"))
  , glow_mid_life(get_uniform<math::vec4>("glow_mid_life"))
  , burn(get_uniform<math::vec3>("burn")) {}

////////////////////////////////////////////////////////////////////////////////

}
