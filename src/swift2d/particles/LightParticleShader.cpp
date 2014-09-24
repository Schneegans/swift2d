////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
      uniform vec4      end_color;
      uniform ivec2     screen_size;

      @include "gbuffer_input"
      @include "get_lit_surface_color"

      layout (location = 0) out vec3 fragColor;

      void main(void) {
        vec4  c = mix(start_color, end_color, age);

        vec4 light        = texture2D(diffuse, texcoords);
        vec3 light_dir    = normalize(light.rgb - 0.5);
        float attenuation = light.a * c.a;

        fragColor = get_lit_surface_color(gl_FragCoord.xy/screen_size, light_dir, c, attenuation);
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
        emit_quad(gl_in[0].gl_Position.xy, vec2(scale));
      }
    )",
    {}
  )
  , projection(get_uniform<math::mat3>("projection"))
  , diffuse(get_uniform<int>("diffuse"))
  , scale(get_uniform<math::vec2>("scale"))
  , start_color(get_uniform<math::vec4>("start_color"))
  , end_color(get_uniform<math::vec4>("end_color"))
  , screen_size(get_uniform<math::vec2i>("screen_size"))
  , g_buffer_diffuse(get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_normal(get_uniform<int>("g_buffer_normal"))
  , g_buffer_light(get_uniform<int>("g_buffer_light")) {}

////////////////////////////////////////////////////////////////////////////////

}
