////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Pipeline.hpp>

#include <swift2d/Swift2D.hpp>
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/components/CameraComponent.hpp>
#include <swift2d/graphics/Window.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/physics.hpp>

#include <thread>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>


namespace swift {

////////////////////////////////////////////////////////////////////////////////

Pipeline::Pipeline()
  : ShadingQuality(5)
  , SuperSampling(false)
  , compositor_(nullptr)
  , max_load_amount_(-1)
  , current_load_amount_(0)
  , needs_reload_(true) {

  ShadingQuality.on_change().connect([this](int){
    needs_reload_ = true;
  });
  SuperSampling.on_change().connect([this](bool){
    needs_reload_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

Pipeline::~Pipeline() {
  if (compositor_) delete compositor_;
  Physics::instance()->clear_gravity_map(window_->get_context());
}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::set_output_window(WindowPtr const& window) {
  window_ = window;

  window_->on_resize.connect([this](math::vec2i){
    needs_reload_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::update() {

  if (max_load_amount_ == -1) {
    Swift2D::instance()->LoadingProgress = 1.f;
  } else {
    Swift2D::instance()->LoadingProgress = 1.f - 1.0f * current_load_amount_ / max_load_amount_;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::draw(ConstSerializedScenePtr const& scene) {

  // create & update window
  if (!window_->Open()) {
    window_->Open = true;
  }

  // update window size
  if (needs_reload_) {
    if (SuperSampling()) {
      window_->get_context().g_buffer_size = window_->get_context().window_size / 2;
    } else {
      window_->get_context().g_buffer_size = window_->get_context().window_size;
    }
    Physics::instance()->create_gravity_map(window_->get_context());

    if (compositor_) {
      delete compositor_;
      compositor_ = nullptr;
    }

    compositor_ = new Compositor(
      window_->get_context(), ShadingQuality(), SuperSampling()
    );
    needs_reload_ = false;
  }

  auto& ctx(window_->get_context());

  // setup projection matrix
  math::mat3 view_matrix(scene->camera->WorldTransform.get());
  math::scale(view_matrix, scene->camera->Size.get());
  ctx.projection_matrix = math::inversed(view_matrix);
  ctx.projection_parallax = scene->camera->Parallax();
  ctx.upload_budget = 1;
  ctx.upload_remaining = 0;

  // compute gravity
  Physics::instance()->update_gravity_map(scene, ctx);

  // draw opaque objects
  compositor_->draw_objects(scene, ctx);

  // draw lights
  compositor_->draw_lights(scene, ctx);

  // perform post processing
  compositor_->post_process(scene, ctx);

  // draw user interface
  compositor_->draw_gui(scene, ctx);

  // update loading state
  if (ctx.upload_remaining > max_load_amount_) {
    max_load_amount_ = ctx.upload_remaining;
  }
  current_load_amount_ = ctx.upload_remaining;
  if (current_load_amount_ == 0) {
    max_load_amount_ = -1;
  }

  // finish frame
  window_->display();
}

////////////////////////////////////////////////////////////////////////////////

}
