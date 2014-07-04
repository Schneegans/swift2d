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
        write_gbuffer(texture2D(diffuse, tex_coords));
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
