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
  : g_buffer_         (new GBuffer(ctx))
  , l_buffer_         (new LBuffer(ctx))
  , post_processor_   (new PostProcessor(ctx)) {}

////////////////////////////////////////////////////////////////////////////////

Compositor::~Compositor() {
  if (post_processor_)    delete post_processor_;
  if (g_buffer_)          delete g_buffer_;
  if (l_buffer_)          delete l_buffer_;
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_objects(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  ogl::Context::BlendFunc(
    oglplus::BlendFunction::SrcAlpha,
    oglplus::BlendFunction::OneMinusSrcAlpha
  );

  g_buffer_->bind_for_drawing(ctx);

  scene->renderers().process_objects(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_lights(ConstSerializedScenePtr const& scene,
                             RenderContext const& ctx) {

  if (ctx.dynamic_lighting) {
    l_buffer_->bind_for_drawing(ctx);
    g_buffer_->bind_normal(1);
    g_buffer_->bind_light(2);

    scene->renderers().process_light(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::post_process(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  post_processor_->process(scene, ctx, g_buffer_, l_buffer_);
}

////////////////////////////////////////////////////////////////////////////////

void Compositor::draw_gui(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {

  if (ctx.sub_sampling) {
    ogl::Context::Viewport(ctx.window_size.x(), ctx.window_size.y());
  }

  ogl::Context::BlendFunc(
    oglplus::BlendFunction::SrcAlpha,
    oglplus::BlendFunction::OneMinusSrcAlpha
  );

  scene->renderers().process_gui(ctx);
}

////////////////////////////////////////////////////////////////////////////////

}
