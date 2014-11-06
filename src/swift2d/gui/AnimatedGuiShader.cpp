////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/AnimatedGuiShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

AnimatedGuiShader::AnimatedGuiShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      // input
      layout(location=0) in vec2 position;

      // uniforms
      uniform vec2 size;
      uniform vec2 offset;

      // varyings
      out vec2 tex_coords;

      void main(void) {
        tex_coords  = vec2(position.x + 1.0, 1.0 - position.y) * 0.5;
        vec2 pos    = position*size + offset;
        gl_Position = vec4(pos, 0.0, 1.0);
      }
    )",
    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      // input
      in vec2 tex_coords;

      // uniforms
      uniform sampler3D diffuse;
      uniform float     time;
      uniform float     opacity;

      // output
      @include "write_gbuffer"

      void main(void){
        vec4 color = texture(diffuse, vec3(tex_coords, time)) * opacity;
        color.rgb /= color.a;
        write_gbuffer(color);
      }
    )"
  )
  , time(get_uniform<float>("time"))
  , opacity(get_uniform<float>("opacity"))
  , size(get_uniform<math::vec2>("size"))
  , offset(get_uniform<math::vec2>("offset"))
  , diffuse(get_uniform<int>("diffuse")) {}

////////////////////////////////////////////////////////////////////////////////

}
