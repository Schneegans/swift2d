////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/NumberParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

NumberParticleShader::NumberParticleShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2 position;
      layout(location=1) in vec2 velocity;
      layout(location=2) in vec2 life;
      layout(location=3) in vec2 rotation;

      out float varying_age;
      out float varying_number;

      void main(void) {
        gl_Position       = vec4(position, 0.0, 1.0);
        varying_age       = life.x;
        varying_number    = rotation.x;
      }
    )",


    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      flat in int number;
      in float age;
      in vec2  texcoords;

      uniform sampler2D font;
      uniform vec4      p_start_color;
      uniform vec4      p_mid_color;
      uniform vec4      p_end_color;
      uniform vec4      n_start_color;
      uniform vec4      n_mid_color;
      uniform vec4      n_end_color;
      uniform vec4      glow_mid_life;
      uniform vec3      burn;

      @include "write_gbuffer"
      @include "three_way_mix"

      void main(void) {
        vec4  p = three_way_mix(p_start_color, p_mid_color, p_end_color, glow_mid_life.w, age);
        vec4  n = three_way_mix(n_start_color, n_mid_color, n_end_color, glow_mid_life.w, age);
        float g = three_way_mix(glow_mid_life.x, glow_mid_life.y, glow_mid_life.z, glow_mid_life.w, age);
        float b = three_way_mix(burn.x, burn.y, burn.z, glow_mid_life.w, age);

        vec4 color = vec4(0);

        int tmp = abs(number);
        int digit_count = 0; do { tmp /= 10; digit_count++; } while (tmp != 0);
        float anchor = 0.5f - digit_count * 0.05f;

        vec2 coords = texcoords + vec2(anchor, 0);

        int digit_index = int(coords.x * 10.0);

        if (digit_index < 10) {
          int digit = abs(number);

          for (int i=10; i>digit_index+1; --i) {
            digit /= 10;
          }

          float offset = 0.1*mod(digit, 10);

          if (digit != 0) {
            color = texture(font, vec2(offset + coords.x - digit_index*0.1, coords.y));
          }
        }

        if (number < 0) color *= n;
        else            color *= p;

        float alpha = color.a;
        vec3 c = color.rgb * alpha;
        alpha = alpha*(1-b);
        float emit = 1.0 * alpha;
        float glow = g * alpha;

        write_gbuffer_premultiplied(vec4(c, alpha), emit, glow);
      }
    )",


    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in float varying_age[];
      in float varying_number[];

      @include "camera_uniforms"

      uniform vec4 glow_mid_life;
      uniform vec4 scale_boost;
      uniform vec2 boost_limits;

      flat out int number;

      out float age;
      out vec2  texcoords;

      @include "emit_quad"
      @include "three_way_mix"

      void main(void) {
        float scale = three_way_mix(scale_boost.x, scale_boost.y, scale_boost.z, glow_mid_life.w, varying_age[0]);
        number = int(varying_number[0]);

        float min = boost_limits.x;
        float max = boost_limits.y;
        float boost = (clamp((abs(varying_number[0]) - min)/(max-min), 0.0, 1.0)) * scale_boost.w + 1;

        emit_quad(gl_in[0].gl_Position.xy, vec2(1.0, 0.135) * scale * boost, 0);
      }
    )",
    {}
  )
  , projection(get_uniform<math::mat3>("projection"))
  , font(get_uniform<int>("font"))
  , boost_limits(get_uniform<math::vec2>("boost_limits"))
  , scale_boost(get_uniform<math::vec4>("scale_boost"))
  , p_start_color(get_uniform<math::vec4>("p_start_color"))
  , p_mid_color(get_uniform<math::vec4>("p_mid_color"))
  , p_end_color(get_uniform<math::vec4>("p_end_color"))
  , n_start_color(get_uniform<math::vec4>("n_start_color"))
  , n_mid_color(get_uniform<math::vec4>("n_mid_color"))
  , n_end_color(get_uniform<math::vec4>("n_end_color"))
  , glow_mid_life(get_uniform<math::vec4>("glow_mid_life"))
  , burn(get_uniform<math::vec3>("burn")) {}

////////////////////////////////////////////////////////////////////////////////

}
