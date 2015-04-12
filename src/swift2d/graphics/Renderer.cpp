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
#define RENDERING_WAIT_FOR_UPDATE 0
#define APPLICATION_FPS 59.f

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
  , application_finished_(false)
  {

  application_step();

  #if MULTITHREADED_RENDERING
    forever_ = boost::thread([&]() {
      while (running_) {
        render_step();
      }
    });
  #else
    render_step();
  #endif
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::stop() {
  running_ = false;
  forever_.join();
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::render_step() {
  {

    #if MULTITHREADED_RENDERING
      std::unique_lock<std::mutex> lock(copy_mutex_);

      while (!updated_scene_) {
        copy_available_.wait(lock);
      }
    #endif


    last_rendered_scene_ = rendered_scene_;
    rendered_scene_ = updated_scene_;

    #if RENDERING_WAIT_FOR_UPDATE
      updated_scene_ = nullptr;
    #endif
  }

  #if MULTITHREADED_RENDERING
    // if (application_finished_) {
    //   application_finished_ = false;
      scheduler_.execute_delayed(0, [this]() {
        application_step();
      });
    // }
  #endif

  Application::get().RenderFPS.step();
  pipeline_.draw(rendered_scene_);
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::application_step() {
  if (!timer_.is_running()) {
    timer_.start();
  }

  double frame_time(timer_.reset());

  Application::get().AppFPS.step();
  Application::get().on_frame.emit(frame_time);
  WindowManager::get().current()->process_input();

  updating_scene_ = SceneManager::get().current()->serialize();
  {
    #if MULTITHREADED_RENDERING
      std::lock_guard<std::mutex> lock(copy_mutex_);
    #endif

    // delete reference to last scene to free memory from main thread
    last_rendered_scene_ = nullptr;
    updated_scene_ = updating_scene_;
  }

  pipeline_.update();

  #if MULTITHREADED_RENDERING
    copy_available_.notify_one();
  #else
    scheduler_.execute_delayed(1.f / APPLICATION_FPS, [this]() {
      application_step();
      render_step();
    });
  #endif


  application_finished_ = true;
}

////////////////////////////////////////////////////////////////////////////////

}
