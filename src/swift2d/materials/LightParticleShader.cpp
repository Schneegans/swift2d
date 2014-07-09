////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/LightParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LightParticleShader::LightParticleShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2  position;
      layout(location=1) in float age;

      out float varying_age;

      void main(void) {
        gl_Position = vec4(position.xy, 0.0, 1.0);
        varying_age = age;
      }
    )",


    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      in float age;
      in vec2  tex_coords;

      // uniforms
      uniform vec3  start_color;
      uniform vec3  end_color;
      uniform float start_opacity;
      uniform float end_opacity;

      uniform sampler2D light_tex;

      @include "gbuffer_input"
      @include "get_lit_surface_color"

      layout (location = 0) out vec3 fragColor;

      void main(void) {
        vec4 light_color = mix(
          vec4(start_color, start_opacity),
          vec4(end_color, end_opacity),
          age
        );

        vec4 light        = texture2D(light_tex, tex_coords);
        vec3 light_dir    = normalize(light.rgb - 0.5);
        float attenuation = light.a * light_color.a;

        fragColor = get_lit_surface_color(light_dir, light_color.rgb, attenuation);
      }
    )",

    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      in float varying_age[];

      uniform mat3  transform;
      uniform mat3  projection;

      uniform float start_scale;
      uniform float end_scale;

      out float age;
      out vec2  tex_coords;

      void main(void) {

        if (varying_age[0] >= 0) {
          float yo[2] = float[2](0.5, -0.5);
          float xo[2] = float[2](0.5, -0.5);

          float scale = mix(start_scale, end_scale, varying_age[0]);

          for(int j=0; j!=2; ++j) {
            for(int i=0; i!=2; ++i) {

              vec2 pos = vec2(xo[i], yo[j]) * scale;

              pos = gl_in[0].gl_Position.xy - pos;
              pos = (projection * transform * vec3(pos, 1.0)).xy;

              gl_Position = vec4(pos, 0.0, 1.0);
              age         = varying_age[0];
              tex_coords  = vec2(-xo[i], yo[j]) + 0.5;
              EmitVertex();
            }
          }
          EndPrimitive();
        }
      }
    )"
  )
  , projection(get_uniform<math::mat3>("projection"))
  , transform(get_uniform<math::mat3>("transform"))
  , screen_size(get_uniform<math::vec2i>("screen_size"))
  , light_tex(get_uniform<int>("light_tex"))
  , start_scale(get_uniform<float>("start_scale"))
  , end_scale(get_uniform<float>("end_scale"))
  , start_color(get_uniform<math::vec3>("start_color"))
  , end_color(get_uniform<math::vec3>("end_color"))
  , start_opacity(get_uniform<float>("start_opacity"))
  , end_opacity(get_uniform<float>("end_opacity"))
  , g_buffer_diffuse(get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_normal(get_uniform<int>("g_buffer_normal"))
  , g_buffer_light(get_uniform<int>("g_buffer_light")) {}

////////////////////////////////////////////////////////////////////////////////

}
