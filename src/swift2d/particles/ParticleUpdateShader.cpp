////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleUpdateShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleUpdateShader::ParticleUpdateShader()
  : Shader(
    R"(
      // vertex shader ---------------------------------------------------------
      @include "version"

      layout(location=0) in vec2  position;
      layout(location=1) in vec2  velocity;
      layout(location=2) in vec2  life;

      out vec2 varying_position;
      out vec2 varying_velocity;
      out vec2 varying_life;

      void main(void) {
        varying_position = position;
        varying_velocity = velocity;
        varying_life = life;
      }
    )",
    "", // empty fragment shader
    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(points) out;
      layout(max_vertices = 30) out;

      in vec2  varying_position[];
      in vec2  varying_velocity[];
      in vec2  varying_life[];

      // general uniforms
      uniform vec2      time;         // x: frame time  y: total time [millisec]

      // spawn uniforms
      uniform sampler2D noise_tex;
      uniform int       spawn_count;
      uniform mat3      transform;
      uniform vec2      life;         // x: life        y: life variance   [sec]
      uniform vec2      direction;    // x: direction   y: direction variance
      uniform vec2      velocity;     // x: velocity    y: velocity variance
      uniform float     position_variance;

      // update uniforms
      uniform sampler2D gravity_map;
      uniform mat3      projection;
      uniform vec3      dynamics;     // x: mass  y: linear damp z: angular damp

      out vec2 out_position;
      out vec2 out_velocity;
      out vec2 out_life;

      vec3 get_random(float seed) {
        seed = mod(seed, 10);
        return (texture(noise_tex, vec2(seed*1.3, seed*2.1)).xyz - 0.5) * 2.0;
      }

      void main(void) {

        if (spawn_count >= 0) {

          // spawn new particles -----------------------------------------------

          for (int i=0; i<spawn_count; ++i) {

            float delta = 1.0*i/spawn_count;
            vec3 random = get_random((i+1)*time.y);

            float l = life.x      + random.x * life.y;
            float d = direction.x + random.y * direction.y;
            float v = velocity.x  + random.z * velocity.y;

            out_position = (transform * vec3(random.xy*position_variance, 1)).xy;
            out_life     = vec2(0, l*1000.0);
            out_velocity = (transform * vec3(cos(d), sin(d), 0)).xy * v;

            EmitVertex(); EndPrimitive();
          }

        } else {

          // update existing particles -----------------------------------------

          if (varying_life[0].x < 1) {

            vec2 texcoords = ((projection * vec3(varying_position[0], 1)).xy + 1.0) * 0.5;

            if (texcoords.x > 0 && texcoords.y > 0 && texcoords.x < 1 && texcoords.y < 1) {
              vec2 gravity = (texture2D(gravity_map, texcoords).rg - 0.5) * dynamics.x;

              out_position = varying_position[0] + varying_velocity[0] * time.x / 1000;
              out_life     = vec2(varying_life[0].x + time.x/varying_life[0].y, varying_life[0].y);
              out_velocity = (varying_velocity[0] + gravity*time.x*0.1) - 0.1 * varying_velocity[0] * dynamics.y * time.x;

              EmitVertex(); EndPrimitive();
            }
          }
        }
      }
    )",
    {"out_position", "out_velocity", "out_life"}
  )
  , time(get_uniform<math::vec2>("time"))
  , noise_tex(get_uniform<int>("noise_tex"))
  , spawn_count(get_uniform<int>("spawn_count"))
  , transform(get_uniform<math::mat3>("transform"))
  , life(get_uniform<math::vec2>("life"))
  , direction(get_uniform<math::vec2>("direction"))
  , velocity(get_uniform<math::vec2>("velocity"))
  , position_variance(get_uniform<float>("position_variance"))
  , gravity_map(get_uniform<int>("gravity_map"))
  , projection(get_uniform<math::mat3>("projection"))
  , dynamics(get_uniform<math::vec3>("dynamics")) {}

////////////////////////////////////////////////////////////////////////////////

}
