////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/trails/ColoredTrailShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ColoredTrailShader::ColoredTrailShader()
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

      @include "write_gbuffer"

      void main(void) {
        vec4  c = mix(start_color, end_color, age);
        float g = mix(glow.x,      glow.y,    age);

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
  , total_time(get_uniform<float>("total_time"))
  , use_global_texcoords(get_uniform<int>("use_global_texcoords"))
  {}

////////////////////////////////////////////////////////////////////////////////

}
