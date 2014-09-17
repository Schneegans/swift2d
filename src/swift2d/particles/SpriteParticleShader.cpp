////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
      uniform vec4      end_color;
      uniform vec2      glow;

      @include "write_gbuffer"

      void main(void) {
        vec4  c = mix(start_color, end_color, age);
        float g = mix(glow.x,      glow.y,    age);

        write_gbuffer(texture2D(diffuse, texcoords) * c, g);
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

      uniform vec2  scale;

      out float age;
      out vec2  texcoords;

      @include "emit_quad"

      void main(void) {
        float scale = mix(scale.x, scale.y, varying_age[0]);
        emit_quad(gl_in[0].gl_Position.xy, scale, varying_rotation[0]);
      }
    )",
    {}
  )
  , projection(get_uniform<math::mat3>("projection"))
  , diffuse(get_uniform<int>("diffuse"))
  , scale(get_uniform<math::vec2>("scale"))
  , start_color(get_uniform<math::vec4>("start_color"))
  , end_color(get_uniform<math::vec4>("end_color"))
  , glow(get_uniform<math::vec2>("glow")) {}

////////////////////////////////////////////////////////////////////////////////

}
