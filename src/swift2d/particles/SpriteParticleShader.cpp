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
#include <swift2d/particles/SpriteParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteParticleShader::SpriteParticleShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2 position;
      layout(location=1) in vec2 velocity;
      layout(location=2) in vec2 life;
      layout(location=3) in vec2 rotation;

      out float varying_age;
      out float varying_rotation;

      void main(void) {
        gl_Position       = vec4(position, 0.0, 1.0);
        varying_age       = life.x;
        varying_rotation  = rotation.x;
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
      uniform vec4      glow_mid_life;
      uniform vec3      burn;

      @include "write_gbuffer"
      @include "three_way_mix"

      void main(void) {
        vec4  c = three_way_mix(start_color, mid_color, end_color, glow_mid_life.w, age);
        float g = three_way_mix(glow_mid_life.x, glow_mid_life.y, glow_mid_life.z, glow_mid_life.w, age);
        float b = three_way_mix(burn.x, burn.y, burn.z, glow_mid_life.w, age);

        vec4 tmp = texture(diffuse, texcoords) * c;
        float alpha = tmp.a;
        vec3 color = tmp.rgb * alpha;
        alpha = alpha*(1-b);
        float emit = 1.0 * alpha;
        float glow = g * alpha;

        write_gbuffer_premultiplied(vec4(color, alpha), emit, glow);
      }
    )",


    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in float varying_age[];
      in float varying_rotation[];

      @include "camera_uniforms"

      uniform vec4 glow_mid_life;
      uniform vec4 start_mid_scale;
      uniform vec2 end_scale;

      out float age;
      out vec2  texcoords;

      @include "emit_quad"
      @include "three_way_mix"

      void main(void) {
        vec2 scale = three_way_mix(start_mid_scale.xy, start_mid_scale.zw, end_scale, glow_mid_life.w, varying_age[0]);
        emit_quad(gl_in[0].gl_Position.xy, scale, varying_rotation[0]);
      }
    )",
    {}
  )
  , projection(get_uniform<math::mat3>("projection"))
  , diffuse(get_uniform<int>("diffuse"))
  , start_mid_scale(get_uniform<math::vec4>("start_mid_scale"))
  , end_scale(get_uniform<math::vec2>("end_scale"))
  , start_color(get_uniform<math::vec4>("start_color"))
  , mid_color(get_uniform<math::vec4>("mid_color"))
  , end_color(get_uniform<math::vec4>("end_color"))
  , glow_mid_life(get_uniform<math::vec4>("glow_mid_life"))
  , burn(get_uniform<math::vec3>("burn")) {}

////////////////////////////////////////////////////////////////////////////////

}
