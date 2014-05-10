////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Pipeline.hpp>

#include <swift2d/graphics/Window.hpp>
#include <swift2d/graphics/Frustum.hpp>
#include <swift2d/scene/Scene.hpp>
#include <swift2d/scene/Transformation.hpp>
#include <swift2d/scene/Sprite.hpp>
#include <swift2d/resources/SpriteResource.hpp>
#include <swift2d/resources/TextureResource.hpp>
#include <swift2d/serializer/SerializedScene.hpp>
#include <swift2d/utils/Logger.hpp>
#include <thread>


namespace swift {

////////////////////////////////////////////////////////////////////////////////

void Pipeline::set_output_window(WindowPtr const& window) {
  window_ = window;
}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::process(std::vector<std::unique_ptr<const Scene>> const& scenes) {

  if (!window_->is_open()) {
    window_->open();
    sprite_ = new SpriteResource();
    tex_ = new TextureResource("diffuse.png");
    window_->get_context().gl.Disable(oglplus::Capability::DepthTest);
  }

  for (auto& s: scenes) {
    auto frustum(Frustum::orthographic(
      camera.eye_left.traverse().get<cores::Transformation::TraversalState>()->world_transform,
      camera.screen_left.traverse().get<cores::Transformation::TraversalState>()->world_transform,
      0.1f, 100.f
    ));

    auto serialized_scene(s->serialize(frustum));

    for (auto& cs: serialized_scene.cores) {
      if (cs.first == typeid(cores::Sprite::SerializedState)) {
        for (auto c: cs.second) {
          auto state = reinterpret_cast<cores::Sprite::SerializedState*>(c);
          auto texture = state->config.texture;
          auto transform = state->transform;

          tex_->bind(window_->get_context(), 0);
          sprite_->draw(window_->get_context());
        }
      }
    }
  }

  window_->display();
}

////////////////////////////////////////////////////////////////////////////////

}
