////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/HeatSpriteShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HeatSpriteShader::HeatSpriteShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "shifted_instanced_texcoords_quad_vertex_shader"
    )",
    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      // varyings
      in vec2 texcoords;
      flat in int instance_id;

      // uniforms
      uniform sampler2D diffuse;
      uniform float     opacity[100];
      uniform mat3      heat_transform[100];

      layout (location = 0) out vec4 fragColor;

      void main(void) {
        vec3 dir = texture(diffuse, texcoords).rga;
        dir.xy = (heat_transform[instance_id] * vec3(dir.xy-0.5, 0.0)).xy + 0.5;
        fragColor = vec4(dir.xy, 0, dir.z*opacity[instance_id]);
      }
    )"
  )
  , projection(get_uniform<math::mat3>("projection"))
  , transform(get_uniform<math::mat3>("transform"))
  , heat_transform(get_uniform<math::mat3>("heat_transform"))
  , depth(get_uniform<float>("depth"))
  , parallax(get_uniform<float>("parallax"))
  , diffuse(get_uniform<int>("diffuse"))
  , texcoord_offset_scale(get_uniform<math::vec4>("texcoord_offset_scale"))
  , opacity(get_uniform<float>("opacity")) {}

////////////////////////////////////////////////////////////////////////////////

}
