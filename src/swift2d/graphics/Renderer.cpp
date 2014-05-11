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
  delete render_client_;
}

////////////////////////////////////////////////////////////////////////////////

Renderer::Renderer(PipelinePtr const& pipeline)
  : render_client_()
  , application_fps_(20) {

  application_fps_.start();

  auto fun = [pipeline, this](ConstSerializedScenePtr const& scene) {
    pipeline->draw(scene);
  };

  render_client_ = new RenderClient<ConstSerializedScenePtr>(fun);
  pipeline->application_fps.connect_from(this->application_fps_.fps);
  pipeline->rendering_fps.connect_from(render_client_->fps_counter.fps);
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::process(SceneObjectPtr const& scene) {

  scene->update();

  render_client_->queue_draw(scene->serialize());

  application_fps_.step();
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::stop() {
  render_client_->stop();
}

////////////////////////////////////////////////////////////////////////////////

}
