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
#include <swift2d/particles/HeatParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HeatParticleShader::HeatParticleShader()
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
      uniform vec3      opacity;
      uniform vec4      scale_mid_life;

      layout (location = 0) out vec4 fragColor;

      @include "three_way_mix"

      void main(void) {
        float o = three_way_mix(opacity.x, opacity.y, opacity.z, scale_mid_life.w, age);
        fragColor = texture(diffuse, texcoords) * vec4(1, 1, 1, o);
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
  , opacity(get_uniform<math::vec3>("opacity")) {}

////////////////////////////////////////////////////////////////////////////////

}
