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
    // vertex shader
    R"(
      @include "quad_vertext_shader"
    )",
    // fragment shader
    R"(
      @include "version"

      // varyings
      in vec2 tex_coords;

      // uniforms
      uniform sampler2D diffuse;

      // output
      layout (location = 0) out vec4 fragColor;
      layout (location = 1) out vec4 fragNormal;
      layout (location = 2) out vec4 fragEmit;

      void main(void){
        fragColor  = texture2D(diffuse, tex_coords);
        fragNormal = vec4(0.5, 0.5, 0, fragColor.a);
        fragEmit   = vec4(1.0, 0, 0, fragColor.a);
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
