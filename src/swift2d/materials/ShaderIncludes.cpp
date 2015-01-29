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

  // ---------------------------------------------------------------------------
  add_include("version", R"(
    #version 330
  )");

  add_include("camera_uniforms", R"(
    uniform mat3  projection;
    uniform float parallax;
  )");


  // ---------------------------------------------------------------------------
  add_include("quad_vertex_shader", R"(
    @include "version"

    // input
    layout(location=0) in vec2 position;

    // uniforms
    @include "camera_uniforms"
    uniform mat3  transform;
    uniform float depth;

    // varyings
    out vec2 texcoords;

    void main(void) {
      vec3 pos    = projection * transform * vec3(position, 1.0) * pow(parallax, depth);
      texcoords   = vec2(position.x + 1.0, 1.0 - position.y) * 0.5;
      gl_Position = vec4(pos.xy, 0.0, 1.0);
    }
  )");

  // ---------------------------------------------------------------------------
  add_include("instanced_quad_vertex_shader", R"(
    @include "version"

    // input
    layout(location=0) in vec2 position;

    // uniforms
    @include "camera_uniforms"
    uniform mat3  transform[100];
    uniform float depth;

    // varyings
    out vec2 texcoords;
    flat out int instance_id;

    void main(void) {
      vec3 pos    = projection * transform[gl_InstanceID] * vec3(position, 1.0) * pow(parallax, depth);
      texcoords   = vec2(position.x + 1.0, 1.0 - position.y) * 0.5;
      instance_id = gl_InstanceID;
      gl_Position = vec4(pos.xy, 0.0, 1.0);
    }
  )");

  // ---------------------------------------------------------------------------
  add_include("shifted_instanced_quad_vertex_shader", R"(
    @include "version"

    // input
    layout(location=0) in vec2 position;

    // uniforms
    @include "camera_uniforms"
    uniform mat3  transform[100];
    uniform float depth;
    uniform vec4  texcoord_offset_scale[100];

    // varyings
    out vec2 texcoords;
    flat out int instance_id;

    void main(void) {
      vec3 pos    = projection * transform[gl_InstanceID] * vec3(position, 1.0) * pow(parallax, depth);
      texcoords   = (vec2(position.x + 1.0, 1.0 - position.y) * 0.5 + texcoord_offset_scale[gl_InstanceID].xy) * texcoord_offset_scale[gl_InstanceID].zw;
      instance_id = gl_InstanceID;
      gl_Position = vec4(pos.xy, 0.0, 1.0);
    }
  )");

  // ---------------------------------------------------------------------------
  add_include("fullscreen_quad_vertex_shader", R"(
    @include "version"

    layout(location=0) in vec2 position;

    // varyings
    out vec2 texcoords;

    void main(void){
      texcoords   = vec2(position.x + 1.0, position.y + 1.0) * 0.5;
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )");

  // ---------------------------------------------------------------------------
  add_include("write_gbuffer", R"(
    layout (location = 0) out vec4 fragColor;
    layout (location = 1) out vec4 fragNormal;
    layout (location = 2) out vec4 fragLight;

    void write_gbuffer(vec4 color, vec4 normal, float emit, float glow, float shinyness) {
      fragColor   = color;
      fragNormal  = vec4(normal.xyz*0.5 + 0.5, normal.a);
      fragLight   = vec4(emit, shinyness, glow, color.a);
    }

    void write_gbuffer(vec4 color, float glow) {
      fragColor   = color;
      fragNormal  = vec4(0.5, 0.5, 0, 0);
      fragLight   = vec4(1.0, 0.0, glow, color.a);
    }

    void write_gbuffer(vec4 color) {
      fragColor   = color;
      fragNormal  = vec4(0.5, 0.5, 0, 0);
      fragLight   = vec4(1.0, 0.0, 0.0, color.a);
    }
  )");

  // ---------------------------------------------------------------------------
  add_include("gbuffer_input", R"(
    uniform sampler2D g_buffer_diffuse;
    uniform sampler2D g_buffer_normal;
    uniform sampler2D g_buffer_light;

    vec3 get_normal(vec2 texcoords) {
      return normalize(texture(g_buffer_normal, texcoords).rgb * 2 - 1);
    }

    vec3 get_diffuse(vec2 texcoords) {
      return texture(g_buffer_diffuse, texcoords).rgb;
    }

    vec3 get_light_info(vec2 texcoords) {
      return texture(g_buffer_light, texcoords).rgb;
    }

  )");

  // ---------------------------------------------------------------------------
  add_include("get_light", R"(
    vec4 get_light(vec2 texcoords, vec3 dir, vec4 color, float gloss, float emit, float attenuation) {
      vec3  normal      = get_normal(texcoords);
      float specular    = pow(max(0, dot(normal, normalize(dir + vec3(0, 0, -1)))), gloss*100 + 1) * gloss;
      float diffuse     = max(0, dot(dir, normal));
      vec3  light       = diffuse * color.rgb;
      return (1.0-emit) * attenuation * vec4(light, specular) * color.a;
    }
  )");

  // ---------------------------------------------------------------------------
  add_include("three_way_mix", R"(
    vec4 three_way_mix(vec4 a, vec4 b, vec4 c, float mid, float alpha) {
      return (alpha < mid ? mix(a, b, alpha/(mid+0.0001)) : mix(b, c, (alpha-mid)/(1.0-mid+0.0001)));
    }

    vec3 three_way_mix(vec3 a, vec3 b, vec3 c, float mid, float alpha) {
      return (alpha < mid ? mix(a, b, alpha/(mid+0.0001)) : mix(b, c, (alpha-mid)/(1.0-mid+0.0001)));
    }

    vec2 three_way_mix(vec2 a, vec2 b, vec2 c, float mid, float alpha) {
      return (alpha < mid ? mix(a, b, alpha/(mid+0.0001)) : mix(b, c, (alpha-mid)/(1.0-mid+0.0001)));
    }

    float three_way_mix(float a, float b, float c, float mid, float alpha) {
      return (alpha < mid ? mix(a, b, alpha/(mid+0.0001)) : mix(b, c, (alpha-mid)/(1.0-mid+0.0001)));
    }
  )");

  // ---------------------------------------------------------------------------
  add_include("emit_quad", R"(
    void emit_quad(vec2 position, vec2 scale, float rotation) {
      const float yo[2] = float[2](0.5, -0.5);
      const float xo[2] = float[2](0.5, -0.5);

      for(int j=0; j!=2; ++j) {
        for(int i=0; i!=2; ++i) {

          vec2 pos = vec2(xo[i], yo[j]) * scale;

          pos = vec2(
            pos.x * cos(rotation) - pos.y * sin(rotation),
            pos.y * cos(rotation) + pos.x * sin(rotation)
          );

          pos = position - pos;
          pos = (projection * vec3(pos, 1.0)).xy;

          gl_Position = vec4(pos, 0.0, 1.0);
          age         = varying_age[0];
          texcoords   = vec2(-xo[i], yo[j]) + 0.5;

          EmitVertex();
        }
      }
      EndPrimitive();
    }

    void emit_quad(vec2 position, vec2 scale) {
      const float yo[2] = float[2](0.5, -0.5);
      const float xo[2] = float[2](0.5, -0.5);

      for(int j=0; j!=2; ++j) {
        for(int i=0; i!=2; ++i) {

          vec2 pos = vec2(xo[i], yo[j]) * scale;
          pos = position - pos;
          pos = (projection * vec3(pos, 1.0)).xy;

          gl_Position = vec4(pos, 0.0, 1.0);
          age         = varying_age[0];
          texcoords   = vec2(-xo[i], yo[j]) + 0.5;

          EmitVertex();
        }
      }
      EndPrimitive();
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
        LOG_WARNING << "Failed to resolve shader include \""
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
