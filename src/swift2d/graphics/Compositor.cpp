////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Compositor.hpp>
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/components/DirectionalLightComponent.hpp>
#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/gui/Interface.hpp>
#include <swift2d/physics/Physics.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Compositor::Compositor(RenderContext const& ctx)
  : g_buffer_         (new GBuffer(ctx, ctx.shading_quality > 0))
  , background_shader_(nullptr)
  , post_processor_   (new PostProcessor(ctx)) {

  if (ctx.shading_quality > 0) {
    background_shader_ = new Shader(
      R"(
        // vertex shader -------------------------------------------------------
        @include "fullscreen_quad_vertex_shader"
      )",
      R"(
        // fragment shader -----------------------------------------------------
        @include "version"

        @include "gbuffer_input"

        uniform vec3 light_dirs[10];
        uniform vec4 light_colors[10];
        uniform int  light_count;

        in vec2 texcoords;

        @include "get_lit_surface_color"

        layout (location = 0) out vec3 fragColor;

        void main(void){
          vec3  light_info = get_light_info(texcoords);
          float emit       = light_info.r;
          vec3  color      = get_diffuse(texcoords);

          if (light_count > 0) {
            vec3  normal     = get_normal(texcoords);
            float gloss      = light_info.g;
            vec3 light_color = vec3(0);

            for (int i=0; i<light_count; ++i) {
              float specular    = max(0, pow(dot(normal, normalize(light_dirs[i] + vec3(0, 0, -1))), gloss*100 + 1) * gloss);
              float diffuse     = max(0, dot(light_dirs[i], normal));
              light_color      += (diffuse*color + specular) * light_colors[i].rgb * light_colors[i].a;
            }

            color = mix(light_color, color, emit);

          } else {
            color = color * emit;
          }

          fragColor = color;
        }
    )");
  }
}

////////////////////////////////////////////////////////////////////////////////

Compositor::~Compositor() {
  if (post_processor_)     delete post_processor_;
  if (background_shader_)  delete background_shader_;
  if (g_buffer_)           delete g_buffer_;
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_objects(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  ctx.gl.BlendFunc(
    oglplus::BlendFunction::SrcAlpha,
    oglplus::BlendFunction::OneMinusSrcAlpha
  );

  g_buffer_->bind_for_drawing(ctx, false);

  scene->renderers().process_objects(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_lights(ConstSerializedScenePtr const& scene,
                             RenderContext const& ctx) {

  if (ctx.shading_quality > 0) {

    ctx.gl.Disable(oglplus::Capability::Blend);

    g_buffer_->bind_final_buffer_for_drawing(ctx, false);
    g_buffer_->bind_diffuse(1);
    g_buffer_->bind_normal(2);
    g_buffer_->bind_light(3);

    background_shader_->use(ctx);
    background_shader_->set_uniform("g_buffer_diffuse", 1);
    background_shader_->set_uniform("g_buffer_normal",  2);
    background_shader_->set_uniform("g_buffer_light",   3);

    std::vector<math::vec3> light_dirs;
    std::vector<math::vec4> light_colors;

    for (auto& light: scene->sun_lights) {
      if (light_dirs.size() < 10) {
        light_dirs.push_back(light.second->Direction());
        light_colors.push_back(light.second->Color().vec4());
      }
    }

    if (light_dirs.size() > 0) {
      background_shader_->set_uniform_array("light_dirs", light_dirs);
      background_shader_->set_uniform_array("light_colors", light_colors);
    }
    background_shader_->set_uniform("light_count",  (int)light_dirs.size());

    Quad::get().draw(ctx);

    ctx.gl.Enable(oglplus::Capability::Blend);
    ctx.gl.BlendFunc(
      oglplus::BlendFunction::One,
      oglplus::BlendFunction::OneMinusSrcColor
    );

    scene->renderers().process_light(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::post_process(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  post_processor_->process(scene, ctx, g_buffer_);
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_gui(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {

  if (ctx.sub_sampling) {
    ctx.gl.Viewport(ctx.window_size.x(), ctx.window_size.y());
  }

  ctx.gl.BlendFunc(
    oglplus::BlendFunction::SrcAlpha,
    oglplus::BlendFunction::OneMinusSrcAlpha
  );

  for (auto& gui: scene->gui_elements) {
    gui.second->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
