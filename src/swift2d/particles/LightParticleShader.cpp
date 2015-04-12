////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/LightParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LightParticleShader::LightParticleShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2 position;
      layout(location=1) in vec2 velocity;
      layout(location=2) in vec2 life;

      out float varying_age;

      void main(void) {
        gl_Position = vec4(position, 0.0, 1.0);
        varying_age = life.x;
      }
    )",

    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      in float age;
      in vec2  texcoords;

      uniform sampler2D diffuse;
      uniform vec4      start_color;
      uniform vec4      mid_color;
      uniform vec4      end_color;
      uniform ivec2     screen_size;
      uniform vec4      scale_mid_life;

      @include "gbuffer_input"
      @include "get_light"
      @include "three_way_mix"

      layout (location = 0) out vec4 fragLight;

      void main(void) {

        vec3  light_info  = get_light_info(gl_FragCoord.xy/screen_size);
        float emit        = light_info.r;

        if (emit >= 1.0) {
          discard;
        }

        float gloss       = light_info.g;

        vec4  c = three_way_mix(start_color, mid_color, end_color, scale_mid_life.w, age);

        vec4 light        = texture(diffuse, texcoords);
        vec3 light_dir    = normalize(light.rgb - 0.5);
        float attenuation = light.a * c.a;

        fragLight = get_light(
          gl_FragCoord.xy/screen_size, light_dir, c,
          gloss, emit, attenuation);
      }
    )",

    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in float varying_age[];

      @include "camera_uniforms"
      uniform vec4 scale_mid_life;

      out float age;
      out vec2  texcoords;

      @include "emit_quad"
      @include "three_way_mix"

      void main(void) {
        float scale = three_way_mix(scale_mid_life.x, scale_mid_life.y, scale_mid_life.z, scale_mid_life.w, varying_age[0]);
        emit_quad(gl_in[0].gl_Position.xy, vec2(scale));
      }
    )",
    {}
  )
  , projection(get_uniform<math::mat3>("projection"))
  , diffuse(get_uniform<int>("diffuse"))
  , scale_mid_life(get_uniform<math::vec4>("scale_mid_life"))
  , start_color(get_uniform<math::vec4>("start_color"))
  , mid_color(get_uniform<math::vec4>("mid_color"))
  , end_color(get_uniform<math::vec4>("end_color"))
  , screen_size(get_uniform<math::vec2i>("screen_size"))
  , g_buffer_normal(get_uniform<int>("g_buffer_normal"))
  , g_buffer_light(get_uniform<int>("g_buffer_light")) {}

////////////////////////////////////////////////////////////////////////////////

}
