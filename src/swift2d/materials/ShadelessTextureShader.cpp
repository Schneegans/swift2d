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
    // vertex shader -----------------------------------------------------------
    R"(
      #version 330

      // input
      layout(location=0) in vec2 position;

      // uniforms
      uniform mat3 projection;
      uniform mat3 transform;

      // varyings
      out vec2 tex_coords;

      void main(void) {
        vec3 pos    = projection * transform * vec3(position, 1.0);
        tex_coords  = position*0.5 + 0.5;
        gl_Position = vec4(pos.xy, 0.0, 1.0);
      }
    )",
    // fragment shader ---------------------------------------------------------
    R"(
      #version 330

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
