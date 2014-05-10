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
#include <swift2d/graphics/RenderClient.hpp>
#include <swift2d/graphics/Pipeline.hpp>
#include <swift2d/utils/Logger.hpp>
#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Renderer::~Renderer() {
  for (auto rc : render_clients_) {
    if (rc) delete rc;
  }
}

////////////////////////////////////////////////////////////////////////////////

Renderer::Renderer(std::vector<PipelinePtr> const& pipelines)
  : render_clients_()
  , application_fps_(20) {

  application_fps_.start();

  for (auto& pipeline : pipelines) {

    auto fun = [pipeline, this](std::vector<ConstSerializedScenePtr> const& scenes) {
      pipeline->draw(scenes);
    };

    auto render_client = new RenderClient<std::vector<ConstSerializedScenePtr>>(fun);
    pipeline->application_fps.connect_from(this->application_fps_.fps);
    pipeline->rendering_fps.connect_from(render_client->fps_counter.fps);
    render_clients_.push_back(render_client);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::process(std::vector<ScenePtr> const& scenes) {

  for (auto& scene: scenes) {
    scene->update();
  }

  std::vector<ConstSerializedScenePtr> serialized_scenes;

  for (auto& scene: scenes) {
    serialized_scenes.push_back(scene->serialize());
  }

  for (auto& rclient : render_clients_) {
    rclient->queue_draw(serialized_scenes);
  }

  application_fps_.step();
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::stop() {

  for (auto& rclient : render_clients_) {
    rclient->stop();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
