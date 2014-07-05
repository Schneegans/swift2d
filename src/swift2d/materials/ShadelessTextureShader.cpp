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
      uniform vec3 colorize;
      uniform float glow;

      // output
      @include "write_gbuffer"

      void main(void) {
        vec4 color = texture2D(diffuse, tex_coords);
        color.rgb *= colorize;
        write_gbuffer(color, 1.0 + glow);
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
