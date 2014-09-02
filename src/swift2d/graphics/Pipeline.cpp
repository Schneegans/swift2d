////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Pipeline.hpp>

#include <swift2d/application/Application.hpp>
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/components/CameraComponent.hpp>
#include <swift2d/graphics/Window.hpp>
#include <swift2d/settings/Settings.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/physics.hpp>

#include <thread>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>


namespace swift {

////////////////////////////////////////////////////////////////////////////////

Pipeline::Pipeline()
  : compositor_(nullptr)
  , max_load_amount_(-1)
  , current_load_amount_(0)
  , needs_reload_(true) {

  Settings::get().Display.ShadingQuality.on_change().connect([this](int) {
    needs_reload_ = true;
  });
  Settings::get().Display.SubSampling.on_change().connect([this](bool) {
    needs_reload_ = true;
  });
  Settings::get().Display.Fullscreen.on_change().connect([this](bool) {
    needs_reload_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

Pipeline::~Pipeline() {
  if (compositor_) delete compositor_;
  Physics::get().clear_gravity_map(window_->get_context());

  for (auto sub_sampler: sub_samplers_) {
    if (sub_sampler) {
      delete sub_sampler;
    }
  }
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
    Application::get().LoadingProgress = 1.f;
  } else {
    Application::get().LoadingProgress = 1.f - 1.0f * current_load_amount_ / max_load_amount_;
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

    window_->get_context().pipeline        = this;
    window_->get_context().shading_quality = Settings::get().Display.ShadingQuality();
    window_->get_context().sub_sampling    = Settings::get().Display.SubSampling();

    if (window_->get_context().sub_sampling) {
      window_->get_context().g_buffer_size = window_->get_context().window_size / 2;
    } else {
      window_->get_context().g_buffer_size = window_->get_context().window_size;
    }
    Physics::get().create_gravity_map(window_->get_context());

    if (compositor_) {
      delete compositor_;
      compositor_ = nullptr;
    }

    compositor_ = new Compositor(window_->get_context());

    for (auto sub_sampler: sub_samplers_) {
      if (sub_sampler) {
        delete sub_sampler;
      }
    }

    sub_samplers_.clear();

    needs_reload_ = false;
  }

  auto& ctx(window_->get_context());

  // setup projection matrix
  math::mat3 view_matrix(scene->camera->WorldTransform.get());
  math::scale(view_matrix, scene->camera->Size.get());
  ctx.projection_matrix   = math::inversed(view_matrix);
  ctx.projection_parallax = scene->camera->Parallax();
  ctx.upload_budget       = 1;
  ctx.upload_remaining    = 0;

  // compute gravity
  Physics::get().update_gravity_map(scene, ctx);

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

SubSampler* Pipeline::get_sub_sampler(int level) {

  int index = level - 2;

  if (sub_samplers_.size() > index && sub_samplers_[index] != nullptr) {
    return sub_samplers_[index];
  }

  if (sub_samplers_.size() <= index) {
    sub_samplers_.resize(index+1);
  }

  sub_samplers_[index] = new SubSampler(window_->get_context(), level);

  return sub_samplers_[index];
}

////////////////////////////////////////////////////////////////////////////////

}
