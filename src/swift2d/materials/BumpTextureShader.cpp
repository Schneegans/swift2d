////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/BumpTextureShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

BumpTextureShader::BumpTextureShader()
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
      uniform mat3 normal_transform;

      uniform sampler2D diffuse;
      uniform sampler2D normal;
      uniform float     emit;
      uniform float     shinyness;
      uniform float     reflectivity;

      @include "write_gbuffer"

      void main(void){
        vec4 out_color  = texture2D(diffuse, tex_coords);
        vec4 out_emit   = vec4(emit, 0, 0, out_color.a);

        vec4 out_normal = texture2D(normal, tex_coords);
        out_normal.xy   = (normal_transform * vec3(out_normal.xy-0.5, 0.0)).xy+0.5;

        write_gbuffer(out_color, out_normal, emit, shinyness, reflectivity);
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
