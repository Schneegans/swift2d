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
        @include "fullscreen_quad_vertext_shader"
      )",
      R"(
        // fragment shader -----------------------------------------------------
        @include "version"

        @include "gbuffer_input"

        in vec2 texcoords;

        layout (location = 0) out vec3 fragColor;

        void main(void){
          fragColor = get_light_info(texcoords).r * get_diffuse(texcoords);
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

  g_buffer_->bind_for_drawing(ctx);

  for (auto& object: scene->objects) {
    object.second->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_lights(ConstSerializedScenePtr const& scene,
                             RenderContext const& ctx) {

  if (ctx.shading_quality > 0) {
    ctx.gl.BlendFunc(
      oglplus::BlendFunction::One,
      oglplus::BlendFunction::OneMinusSrcColor
    );

    g_buffer_->bind_final_buffer_for_drawing(ctx);

    g_buffer_->bind_diffuse(1);
    g_buffer_->bind_normal(2);
    g_buffer_->bind_light(3);

    background_shader_->use(ctx);
    background_shader_->set_uniform("g_buffer_diffuse", 1);
    background_shader_->set_uniform("g_buffer_light", 3);
    Quad::get().draw(ctx);

    for (auto& light: scene->lights) {
      light.second->draw(ctx);
    }
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
