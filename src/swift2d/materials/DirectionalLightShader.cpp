////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/DirectionalLightShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DirectionalLightShader::DirectionalLightShader()
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
      uniform vec3 light_dir;

      @include "gbuffer_input"
      @include "write_lbuffer"
      @include "light_helpers"

      void main(void){
        vec3 normal       = get_normal();
        vec3 surface_dir  = normalize(normal - 0.5);

        float spot        = get_specular_light(light_dir, surface_dir);
        float intensity   = get_diffuse_light(light_dir, surface_dir);

        write_lbuffer(vec3(intensity, spot, 0));
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
