////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/shader_snippets.hpp>

namespace swift {
namespace shader_snippets {

////////////////////////////////////////////////////////////////////////////////

std::string get_quad_vertext_shader() {
  return R"(
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
  )";
}

////////////////////////////////////////////////////////////////////////////////

}
}
