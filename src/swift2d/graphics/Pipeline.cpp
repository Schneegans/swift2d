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
#include <swift2d/settings/SettingsWrapper.hpp>
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

  SettingsWrapper::get().Settings->DynamicLighting.on_change().connect([this](int) {
    needs_reload_ = true;
    return true;
  });
  SettingsWrapper::get().Settings->SubSampling.on_change().connect([this](bool) {
    needs_reload_ = true;
    return true;
  });
  SettingsWrapper::get().Settings->LightSubSampling.on_change().connect([this](bool) {
    needs_reload_ = true;
    return true;
  });
  SettingsWrapper::get().Settings->LensFlares.on_change().connect([this](bool) {
    needs_reload_ = true;
    return true;
  });
  SettingsWrapper::get().Settings->HeatEffect.on_change().connect([this](bool) {
    needs_reload_ = true;
    return true;
  });
  SettingsWrapper::get().Settings->Fullscreen.on_change().connect([this](bool) {
    needs_reload_ = true;
    return true;
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

  window_->Size.on_change().connect([this](math::vec2i){
    needs_reload_ = true;
    return true;
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

  SWIFT_PUSH_GL_RANGE("Frame");

  // create & update window
  window_->set_active(true);
  window_->init_context();

  // update window size
  if (needs_reload_) {
    window_->get_context().pipeline           = this;
    window_->get_context().sub_sampling       = SettingsWrapper::get().Settings->SubSampling();
    window_->get_context().dynamic_lighting   = SettingsWrapper::get().Settings->DynamicLighting();
    window_->get_context().light_sub_sampling = SettingsWrapper::get().Settings->LightSubSampling();
    window_->get_context().lens_flares        = SettingsWrapper::get().Settings->LensFlares();
    window_->get_context().heat_effect        = SettingsWrapper::get().Settings->HeatEffect();

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
  SWIFT_PUSH_GL_RANGE("Update gravity map");
  Physics::get().update_gravity_map(scene, ctx);
  SWIFT_POP_GL_RANGE();

  // draw opaque objects
  SWIFT_PUSH_GL_RANGE("Draw objects");
  compositor_->draw_objects(scene, ctx);
  SWIFT_POP_GL_RANGE();

  // draw lights
  SWIFT_PUSH_GL_RANGE("Draw lights");
  compositor_->draw_lights(scene, ctx);
  SWIFT_POP_GL_RANGE();

  // perform post processing
  SWIFT_PUSH_GL_RANGE("Post process");
  compositor_->post_process(scene, ctx);
  SWIFT_POP_GL_RANGE();

  // draw user interface
  SWIFT_PUSH_GL_RANGE("Draw GUI");
  compositor_->draw_gui(scene, ctx);
  SWIFT_POP_GL_RANGE();

  // update loading state
  if (ctx.upload_remaining > max_load_amount_) {
    max_load_amount_ = ctx.upload_remaining;
  }
  current_load_amount_ = ctx.upload_remaining;
  if (current_load_amount_ == 0) {
    max_load_amount_ = -1;
  }

  SWIFT_POP_GL_RANGE();

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
