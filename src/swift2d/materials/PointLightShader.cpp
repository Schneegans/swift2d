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

      @include "gbuffer_input"
      @include "write_lbuffer"
      @include "light_helpers"

      void main(void){
        vec3 normal       = get_normal();
        vec4 light        = texture2D(light_tex, tex_coords);

        vec3 light_dir    = normalize(light.rgb - 0.5);
        vec3 surface_dir  = normalize(normal - 0.5);

        float spot        = get_specular_light(light_dir, surface_dir) * light.a;
        float intensity   = get_diffuse_light(light_dir, surface_dir) * light.a;

        write_lbuffer(vec3(intensity, spot, 0));
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
