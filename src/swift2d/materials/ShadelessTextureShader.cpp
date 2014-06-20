////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/ShadelessTextureShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ShadelessTextureShader::ShadelessTextureShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "quad_vertext_shader"
    )",
    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      // input
      in vec2 tex_coords;

      // uniforms
      uniform sampler2D diffuse;

      // output
      @include "write_gbuffer"

      void main(void){
        vec4 out_color  = texture2D(diffuse, tex_coords);
        vec4 out_normal = vec4(0.5, 0.5, 0, out_color.a);
        vec4 out_emit   = vec4(1.0, 0, 0, out_color.a);

        write_gbuffer(out_color, out_normal, out_emit);
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
