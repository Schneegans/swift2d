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

      layout(location=0) in vec2 position;
      layout(location=1) in vec2 velocity;
      layout(location=2) in vec2 life;
      layout(location=3) in vec2 rotation;

      out vec2 varying_position;
      out vec2 varying_velocity;
      out vec2 varying_life;
      out vec2 varying_rotation;

      void main(void) {
        varying_position  = position;
        varying_velocity  = velocity;
        varying_life      = life;
        varying_rotation  = rotation;
      }
    )",
    "", // empty fragment shader
    R"(
      // geometry shader -------------------------------------------------------
      @include "version"

      layout(points) in;
      layout(points) out;
      layout(max_vertices = 50) out;

      in vec2 varying_position[];
      in vec2 varying_velocity[];
      in vec2 varying_life[];
      in vec2 varying_rotation[];

      // general uniforms
      uniform vec2      time;         // x: frame time  y: total time [millisec]

      // spawn uniforms
      uniform sampler2D noise_tex;
      uniform ivec2     spawn_count_it;
      uniform vec3      transform[50];  // xy: pos        z: rot
      uniform vec2      life;           // x: life        y: life variance   [sec]
      uniform vec2      velocity;       // x: velocity    y: velocity variance
      uniform vec2      rotation;       // x: rotation    y: rotation variance
      uniform vec3      dir_pos_rot_variance;

      // update uniforms
      uniform sampler2D gravity_map;
      uniform mat3      projection;
      uniform vec3      dynamics;     // x: mass  y: linear damp z: angular damp

      out vec2 out_position;
      out vec2 out_velocity;
      out vec2 out_life;
      out vec2 out_rotation;

      vec3 get_random(vec2 seed) {
        return (texture(noise_tex, vec2(seed.x, seed.y)).xyz - 0.5) * 2.0;
      }

      void main(void) {

        if (spawn_count_it.x >= 0) {

          // spawn new particles -----------------------------------------------

          for (int i=0; i<spawn_count_it.x; ++i) {

            float delta = 1.0*i/spawn_count_it.x;
            vec3 random1 = get_random(vec2((delta+spawn_count_it.y+1) * time.y, (delta+spawn_count_it.y+1) * time.x));
            vec3 random2 = get_random(vec2((delta+spawn_count_it.y+2) * time.y, (delta+spawn_count_it.y+2) * time.x));

            float l = max(0, life.x  + random1.x * life.y);
            float d = transform[i].z + random1.y * dir_pos_rot_variance.z + dir_pos_rot_variance.x;
            float v = velocity.x  + random1.z * velocity.y;
            float r = rotation.x  + random1.z * rotation.y;

            out_position = transform[i].xy + random2.xy*dir_pos_rot_variance.y;
            out_life     = vec2(0, l*1000.0);
            out_velocity = vec2(cos(d), sin(d)) * v;
            out_rotation = vec2(d, r);

            EmitVertex(); EndPrimitive();
          }

        } else {

          // update existing particles -----------------------------------------

          if (varying_life[0].x < 1) {

            vec2 texcoords = ((projection * vec3(varying_position[0], 1)).xy + 1.0) * 0.5;

            float clip_dist = 0.3;

            if (texcoords.x > -clip_dist && texcoords.y > -clip_dist && texcoords.x < 1+clip_dist && texcoords.y < 1+clip_dist) {
              vec2 gravity = (texture(gravity_map, texcoords).rg - 0.5) * dynamics.x;

              out_position = varying_position[0] + varying_velocity[0] * time.x / 1000;
              out_life     = vec2(varying_life[0].x + time.x/varying_life[0].y, varying_life[0].y);
              out_velocity = (varying_velocity[0] + gravity*time.x*0.1) - 0.01 * varying_velocity[0] * dynamics.y * time.x;
              out_rotation = vec2(varying_rotation[0].x + varying_rotation[0].y * time.x / 1000, varying_rotation[0].y - 0.01 * varying_rotation[0].y * dynamics.z * time.x);

              EmitVertex(); EndPrimitive();
            }
          }
        }
      }
    )",
    {"out_position", "out_velocity", "out_life", "out_rotation"}
  )
  , time(get_uniform<math::vec2>("time"))
  , noise_tex(get_uniform<int>("noise_tex"))
  , spawn_count_it(get_uniform<math::vec2i>("spawn_count_it"))
  , transform(get_uniform<math::vec3>("transform"))
  , life(get_uniform<math::vec2>("life"))
  , dir_pos_rot_variance(get_uniform<math::vec3>("dir_pos_rot_variance"))
  , velocity(get_uniform<math::vec2>("velocity"))
  , rotation(get_uniform<math::vec2>("rotation"))
  , gravity_map(get_uniform<int>("gravity_map"))
  , projection(get_uniform<math::mat3>("projection"))
  , dynamics(get_uniform<math::vec3>("dynamics")) {}

////////////////////////////////////////////////////////////////////////////////

}
