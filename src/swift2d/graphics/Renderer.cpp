////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/graphics/Renderer.hpp>

#include <swift2d/application/Application.hpp>
#include <swift2d/scene.hpp>
#include <swift2d/graphics/Pipeline.hpp>
#include <swift2d/graphics/WindowManager.hpp>
#include <swift2d/utils/Logger.hpp>
#include <thread>
#include <memory>

#define MULTITHREADED_RENDERING 1

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Renderer::~Renderer() {
  stop();
}

////////////////////////////////////////////////////////////////////////////////

Renderer::Renderer(Pipeline& pipeline)
  : pipeline_(pipeline)
  , rendered_scene_()
  , updating_scene_()
  , updated_scene_()
  , running_(true)
  , started_rendering_(true) {

  application_step();

  #if MULTITHREADED_RENDERING
    forever_ = boost::thread([&]() {
      while (running_) {
        render_step();
      }
    });
  #endif
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::stop() {
  running_ = false;
  forever_.join();
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::render_step() {
  if (updated_scene_) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      last_rendered_scene_ = rendered_scene_;
      rendered_scene_ = updated_scene_;
      started_rendering_ = true;
    }

    Application::get().RenderFPS.step();
    pipeline_.draw(rendered_scene_);
  } else {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::application_step() {
  if (started_rendering_) {
    if (!timer_.is_running()) {
      timer_.start();
    }
    started_rendering_ = false;

    scheduler_.execute_delayed(1.0 / 60.0, [this]() {
      application_step();
    });

    Application::get().AppFPS.step();
    Application::get().on_frame.emit(timer_.reset());
    WindowManager::get().current()->process_input();

    updating_scene_ = SceneManager::get().current()->serialize();
    {
      std::unique_lock<std::mutex> lock(mutex_);

      // delete reference to last scene to free memory from main thread
      last_rendered_scene_ = nullptr;

      updated_scene_ = updating_scene_;
    }

    pipeline_.update();

    #if !MULTITHREADED_RENDERING
    render_step();
    #endif

  } else {
    scheduler_.execute_delayed(1.0 / 1000.0, [this]() {
      application_step();
    });
  }
}

////////////////////////////////////////////////////////////////////////////////

}
