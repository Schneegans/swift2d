////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/graphics/Renderer.hpp>

#include <swift2d/scene.hpp>
#include <swift2d/graphics/Pipeline.hpp>
#include <swift2d/utils/Logger.hpp>
#include <thread>
#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Renderer::~Renderer() {}

////////////////////////////////////////////////////////////////////////////////

Renderer::Renderer(PipelinePtr const& pipeline)
  : rendered_scene_()
  , updating_scene_()
  , updated_scene_()
  , AppFPS(20)
  , RenderFPS(20)
  , running_(true) {

  AppFPS.start();
  RenderFPS.start();

  forever_ = boost::thread([&]() {

    while (running_) {
      {
        std::unique_lock<std::mutex> lock(mutex_);
        rendered_scene_ = updated_scene_;
      }

      RenderFPS.step();

      if (rendered_scene_) {
        pipeline->draw(rendered_scene_);
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::process(SceneObjectPtr const& scene, CameraComponentPtr const& camera) {
  AppFPS.step();

  updating_scene_ = scene->serialize(camera);

  std::unique_lock<std::mutex> lock(mutex_);
  updated_scene_ = updating_scene_;
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::stop() {
  running_ = false;
  forever_.join();
}

////////////////////////////////////////////////////////////////////////////////

}
