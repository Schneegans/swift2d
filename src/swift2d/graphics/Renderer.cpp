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

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Renderer::~Renderer() {
  stop();
}

////////////////////////////////////////////////////////////////////////////////

Renderer::Renderer(Pipeline& pipeline)
  : rendered_scene_()
  , updating_scene_()
  , updated_scene_()
  , running_(true)
  , started_rendering_(true) {

  ticker_ = Ticker::create(1.0 / 1000.0);
  ticker_->on_tick.connect([&]() {
    if (started_rendering_) {
      started_rendering_ = false;
      updating_scene_ = SceneManager::get().current_scene()->serialize(SceneManager::get().current_camera());
      {
        std::unique_lock<std::mutex> lock(mutex_);
        updated_scene_ = updating_scene_;
      }

      Application::get().AppFPS.step();
      Application::get().on_frame.emit();
      WindowManager::get().current()->process_input();

      pipeline.update();
    }
  });
  ticker_->start();

  forever_ = boost::thread([&]() {

    // Timer timer;
    // timer.start();

    while (running_) {
      if (updated_scene_) {
        // ticker_->set_tick_time(timer.reset());
        // std::cout << ticker_->get_tick_time() << std::endl;
        {
          std::unique_lock<std::mutex> lock(mutex_);
          rendered_scene_ = updated_scene_;
          started_rendering_ = true;
        }

        Application::get().RenderFPS.step();
        pipeline.draw(rendered_scene_);
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::stop() {
  running_ = false;
  forever_.join();
}

////////////////////////////////////////////////////////////////////////////////

}
