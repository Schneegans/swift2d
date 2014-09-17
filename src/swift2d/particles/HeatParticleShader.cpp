////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
      uniform vec2      opacity;

      layout (location = 0) out vec4 fragColor;

      void main(void) {
        float o = mix(opacity.x, opacity.y, age);
        fragColor = texture2D(diffuse, texcoords) * vec4(1, 1, 1, o);
      }
    )",


    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in float varying_age[];

      @include "camera_uniforms"
      uniform vec2 scale;

      out float age;
      out vec2  texcoords;

      @include "emit_quad"

      void main(void) {
        float scale = mix(scale.x, scale.y, varying_age[0]);
        emit_quad(gl_in[0].gl_Position.xy, scale);
      }
    )",
    {}
  )
  , projection(get_uniform<math::mat3>("projection"))
  , diffuse(get_uniform<int>("diffuse"))
  , scale(get_uniform<math::vec2>("scale"))
  , opacity(get_uniform<math::vec2>("opacity")) {}

////////////////////////////////////////////////////////////////////////////////

}
