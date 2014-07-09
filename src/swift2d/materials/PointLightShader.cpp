////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/PointLightShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointLightShader::PointLightShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "quad_vertext_shader"
    )",
    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      // varyings
      in vec2 tex_coords;

      // uniforms
      uniform sampler2D light_tex;
      uniform vec3      light_color;

      @include "gbuffer_input"
      @include "get_lit_surface_color"

      layout (location = 0) out vec3 fragColor;

      void main(void){
        vec4 light        = texture2D(light_tex, tex_coords);
        vec3 light_dir    = normalize(light.rgb - 0.5);
        float attenuation = light.a;

        fragColor = get_lit_surface_color(light_dir, light_color, attenuation);
      }
    )"
  )
  , projection(get_uniform<math::mat3>("projection"))
  , transform(get_uniform<math::mat3>("transform"))
  , depth(get_uniform<float>("depth"))
  , parallax(get_uniform<float>("parallax"))
  , screen_size(get_uniform<math::vec2i>("screen_size"))
  , g_buffer_diffuse(get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_normal(get_uniform<int>("g_buffer_normal"))
  , g_buffer_aux_1(get_uniform<int>("g_buffer_aux_1"))
  , light_tex(get_uniform<int>("light_tex"))
  , light_color(get_uniform<math::vec3>("light_color")) {}

////////////////////////////////////////////////////////////////////////////////

}
