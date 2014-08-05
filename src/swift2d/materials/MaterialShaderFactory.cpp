////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/MaterialShaderFactory.hpp>

#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

MaterialShaderPtr MaterialShaderFactory::get_shader(int capabilities) {
  auto shader(shaders_.find(capabilities));

  if (shader != shaders_.end()) {
    return shader->second;
  }

  // else create new shader
  std::string v_shader(R"(
    @include "quad_vertext_shader"
  )");

  std::stringstream f_shader;

  f_shader << R"(
    @include "version"
    in vec2 tex_coords;
    @include "write_gbuffer"

    uniform float time;
  )";

  if (capabilities & ANIMATED_DIFFUSE_TEX) {
    f_shader << R"(
      uniform sampler3D diffuse_tex;
      uniform vec4 diffuse;
      vec4 get_diffuse() {
        vec4 result = texture(diffuse_tex, vec3(tex_coords, time));
        result *= diffuse;
        return result;
      }
    )";
  } else if (capabilities & DIFFUSE_TEX) {
    f_shader << R"(
      uniform sampler2D diffuse_tex;
      uniform vec4 diffuse;
      vec4 get_diffuse() {
        vec4 result = texture2D(diffuse_tex, tex_coords);
        result *= diffuse;
        return result;
      }
    )";
  } else {
    f_shader << R"(
      uniform vec4 diffuse;
      vec4 get_diffuse() {
        return diffuse;
      }
    )";
  }

  if (capabilities & ANIMATED_NORMAL_TEX) {
    f_shader << R"(
      uniform sampler3D normal_tex;
      uniform mat3 normal_transform;
      vec4 get_normal() {
        vec4 result = texture(normal_tex, tex_coords, time) - vec4(0.5, 0.5, 0.5, 0);
        result.xy   = (normal_transform * vec3(result.xy, 0.0)).xy;
        // result.xyz  = normalize(result.xyz);
        return result;
      }
    )";
  } else if (capabilities & NORMAL_TEX) {
    f_shader << R"(
      uniform sampler2D normal_tex;
      uniform mat3 normal_transform;
      vec4 get_normal() {
        vec4 result = texture2D(normal_tex, tex_coords) - vec4(0.5, 0.5, 0.5, 0);
        result.xy   = (normal_transform * vec3(result.xy, 0.0)).xy;
        // result.xyz  = normalize(result.xyz);
        return result;
      }
    )";
  } else {
    f_shader << R"(
      vec4 get_normal() {
        return vec4(0, 0, -1, 0);
      }
    )";
  }

  auto append_float_parameter = [&](std::string const& name, Capabilities cap, Capabilities anim_cap) {
    if (capabilities & anim_cap) {
      f_shader << "uniform sampler3D " << name << "_tex;"          << std::endl;
      f_shader << "uniform float " << name << ";"                  << std::endl;
      f_shader << "float get_" << name << "() {"                   << std::endl;
      f_shader << "  return texture(" << name << "_tex, tex_coords, time).r * "
                                      << name << ";"               << std::endl;
      f_shader << "}"                                              << std::endl;
    } else if (capabilities & cap) {
      f_shader << "uniform sampler2D " << name << "_tex;"          << std::endl;
      f_shader << "uniform float " << name << ";"                  << std::endl;
      f_shader << "float get_" << name << "() {"                   << std::endl;
      f_shader << "  return texture2D(" << name << "_tex, tex_coords).r * "
                                        << name << ";"             << std::endl;
      f_shader << "}"                                              << std::endl;
    } else {
      f_shader << "uniform float " << name << ";"                  << std::endl;
      f_shader << "float get_" << name << "() {"                   << std::endl;
      f_shader << "  return " << name << ";"                       << std::endl;
      f_shader << "}"                                              << std::endl;
    }
  };

  append_float_parameter("emit",      EMIT_TEX,       ANIMATED_EMIT_TEX);
  append_float_parameter("glow",      GLOW_TEX,       ANIMATED_GLOW_TEX);
  append_float_parameter("shinyness", SHINYNESS_TEX,  ANIMATED_SHINYNESS_TEX);

  f_shader << R"(
    void main(void) {
      write_gbuffer(
        get_diffuse(), get_normal(), get_emit(), get_glow(), get_shinyness()
      );
    }
  )";

  auto new_shader = MaterialShader::create(capabilities, v_shader, f_shader.str());
  shaders_[capabilities] = new_shader;

  return new_shader;
}

////////////////////////////////////////////////////////////////////////////////

}
