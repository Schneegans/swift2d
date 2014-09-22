////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/trails/TexturedTrailShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TexturedTrailShader::TexturedTrailShader()
  : TrailShaderBase(),
    Shader(
    R"(
      @include "trail_vertex_shader"
    )",

    R"(
      // fragment shader -------------------------------------------------------
      @include "version"

      in vec2 texcoords;
      in float age;

      uniform vec4 start_color;
      uniform vec4 end_color;
      uniform vec2 glow;

      uniform sampler2D texture;
      uniform float texture_repeat;

      @include "write_gbuffer"

      void main(void) {
        vec4 color = texture2D(texture, vec2(texcoords.x * texture_repeat, texcoords.y));
        vec4  c = color * mix(start_color, end_color, age);
        float g =         mix(glow.x,      glow.y,    age);

        write_gbuffer(c, g);
      }
    )",


    R"(
      // geometry shader -------------------------------------------------------
      @include "trail_tf_shader"
    )"
  )
  , projection(get_uniform<math::mat3>("projection"))
  , start_width(get_uniform<float>("start_width"))
  , end_width(get_uniform<float>("end_width"))
  , start_color(get_uniform<math::vec4>("start_color"))
  , end_color(get_uniform<math::vec4>("end_color"))
  , glow(get_uniform<math::vec2>("glow"))
  , texture(get_uniform<int>("texture"))
  , texture_repeat(get_uniform<float>("texture_repeat"))
  , total_time(get_uniform<float>("total_time"))
  , use_global_texcoords(get_uniform<int>("use_global_texcoords"))
  {}

////////////////////////////////////////////////////////////////////////////////

}
