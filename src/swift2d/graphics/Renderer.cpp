////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/graphics/Renderer.hpp>

// guacamole headers
#include <swift2d/scene.hpp>
#include <swift2d/graphics/RenderClient.hpp>
#include <swift2d/graphics/Pipeline.hpp>
#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

namespace {

std::shared_ptr<Renderer::const_scene_vec_t> garbage_collected_copy(
    std::vector<Scene const*> const& scenes) {

  auto sgs = std::make_shared<Renderer::scene_vec_t>();
  for (auto scene : scenes) {
    sgs->push_back(std::unique_ptr<Scene>(new Scene(scene->deep_copy())));
  }
  return sgs;
}

}

////////////////////////////////////////////////////////////////////////////////

Renderer::~Renderer() {
  for (auto rc : render_clients_) {
    if (rc) delete rc;
  }
}

////////////////////////////////////////////////////////////////////////////////

Renderer::Renderer(std::vector<PipelinePtr> const& pipelines)
    : render_clients_(),
      application_fps_(20) {

  application_fps_.start();

  for (auto& pipeline : pipelines) {

    auto fun = [pipeline, this](std::shared_ptr<const_scene_vec_t> const& sg) {
      pipeline->process(*sg);
    };

    auto render_client = new render_client_t(fun);
    pipeline->application_fps.connect_from(this->application_fps_.fps);
    pipeline->rendering_fps.connect_from(render_client->fps_counter.fps);
    render_clients_.push_back(render_client);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Renderer::queue_draw(std::vector<Scene const*> const& scenes) {

  // for (auto scene : scenes) {
  //   scene->update_cache();
  // }

  auto sgs = garbage_collected_copy(scenes);
  for (auto& rclient : render_clients_) {
    rclient->queue_draw(sgs);
  }

  application_fps_.step();
}

////////////////////////////////////////////////////////////////////////////////

}
