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
      uniform sampler2D normal;
      uniform float     emit;

      // output
      layout (location = 0) out vec4 fragColor;
      layout (location = 1) out vec4 fragNormal;
      layout (location = 2) out vec4 fragEmit;

      void main(void){
        fragColor  = texture2D(diffuse, tex_coords);
        fragNormal  = texture2D(normal, tex_coords);
        fragEmit   = vec4(emit, 0, 0, fragColor.a);
      }
    )"
  ) {}

////////////////////////////////////////////////////////////////////////////////

}
