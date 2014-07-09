////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/ShaderIncludes.hpp>
#include <swift2d/utils/Logger.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ShaderIncludes::ShaderIncludes() {

  add_include("quad_vertext_shader", R"(
    @include "version"

    // input
    layout(location=0) in vec2 position;

    // uniforms
    uniform mat3  projection;
    uniform mat3  transform;
    uniform float depth;
    uniform float parallax;

    // varyings
    out vec2 tex_coords;

    void main(void) {
      vec3 pos    = projection * transform * vec3(position, 1.0) * pow(parallax, depth);
      tex_coords  = vec2(position.x + 1.0, 1.0 - position.y) * 0.5;
      gl_Position = vec4(pos.xy, 0.0, 1.0);
    }
  )");

  add_include("fullscreen_quad_vertext_shader", R"(
    @include "version"

    layout(location=0) in vec2 position;

    void main(void){
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )");

  add_include("version", R"(
    #version 330
  )");

  add_include("write_gbuffer", R"(
    layout (location = 0) out vec4 fragColor;
    layout (location = 1) out vec4 fragNormal;
    layout (location = 2) out vec4 fragAux1;
    layout (location = 3) out vec4 fragAux2;

    void write_gbuffer(vec4 color, vec4 normal, float emit, float glow, float shinyness, float reflectivity) {
      fragColor   = color;
      fragNormal  = normal;
      fragAux1    = vec4(emit, shinyness, reflectivity, color.a);
      fragAux2    = vec4(glow, 0.0, 0.0, color.a);
    }

    void write_gbuffer(vec4 color, float glow) {
      fragColor   = color;
      fragNormal  = vec4(0.5, 0.5, 0, 0);
      fragAux1    = vec4(1.0, 1.0, 1.0, color.a);
      fragAux2    = vec4(glow, 0.0, 0.0, color.a);
    }

    void write_gbuffer(vec4 color) {
      fragColor   = color;
      fragNormal  = vec4(0.5, 0.5, 0, 0);
      fragAux1    = vec4(1.0, 1.0, 1.0, color.a);
      fragAux2    = vec4(0.0, 0.0, 0.0, color.a);
    }
  )");

  add_include("write_lbuffer", R"(
    layout (location = 0) out vec4 fragColor;

    void write_lbuffer(vec3 color) {
      fragColor = vec4(color, 1.0);
    }
  )");

  add_include("gbuffer_input", R"(
    uniform ivec2     screen_size;
    uniform sampler2D g_buffer_diffuse;
    uniform sampler2D g_buffer_normal;
    uniform sampler2D g_buffer_aux_1;
    uniform sampler2D g_buffer_aux_2;

    vec3 get_normal() {
      return texture2D(g_buffer_normal, gl_FragCoord.xy/screen_size).rgb;
    }

    vec3 get_diffuse() {
      return texture2D(g_buffer_diffuse, gl_FragCoord.xy/screen_size).rgb;
    }

    float get_emit() {
      return texture2D(g_buffer_aux_1, gl_FragCoord.xy/screen_size).r;
    }

    float get_shinyness() {
      return texture2D(g_buffer_aux_1, gl_FragCoord.xy/screen_size).g;
    }

    float get_reflectivity() {
      return texture2D(g_buffer_aux_1, gl_FragCoord.xy/screen_size).b;
    }

    float get_glow() {
      return texture2D(g_buffer_aux_2, gl_FragCoord.xy/screen_size).r;
    }

  )");

  add_include("light_helpers", R"(
    float get_specular_light(vec3 light_dir, vec3 surface_dir) {
      return pow(max(0, dot(vec3(0, 0, 1), reflect(light_dir, surface_dir))), get_shinyness()*100);
    }

    float get_diffuse_light(vec3 light_dir, vec3 surface_dir) {
      return max(0, dot(light_dir, surface_dir));
    }
  )");
}

////////////////////////////////////////////////////////////////////////////////

void ShaderIncludes::process(std::string& input) const {
  while (process_impl(input));
}

////////////////////////////////////////////////////////////////////////////////

void ShaderIncludes::add_include(std::string const& identifier,
                                 std::string const& code) {
  snippets_[identifier] = code;
}

////////////////////////////////////////////////////////////////////////////////

bool ShaderIncludes::process_impl(std::string& input) const {
  std::size_t search_pos(0);
  std::string search("@include");

  bool replaced_something(false);

  while(search_pos != std::string::npos) {
    // find include
    search_pos = input.find(search, search_pos);

    if (search_pos != std::string::npos) {

      // get file name
      std::size_t start(input.find('\"', search_pos)+1);
      std::size_t end (input.find('\"', start));

      std::string snippet(input.substr(start, end-start));

      // get included file
      std::string include;

      auto code = snippets_.find(snippet);
      if (code != snippets_.end()) {
        include = code->second;
      } else {
        Logger::LOG_WARNING << "Failed to resolve shader include \""
                            << snippet << "\": No such snippet!" << std::endl;
      }

      // include it
      input.replace(search_pos, end-search_pos + 2, include);

      // advance search pos
      search_pos = search_pos + include.length();

      replaced_something = true;
    }
  }

  return replaced_something;
}

////////////////////////////////////////////////////////////////////////////////


}
