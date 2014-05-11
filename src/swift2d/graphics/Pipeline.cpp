////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Pipeline.hpp>

#include <swift2d/components/SpriteComponent.hpp>
#include <swift2d/graphics/Window.hpp>
#include <swift2d/utils/Logger.hpp>
#include <thread>


namespace swift {

////////////////////////////////////////////////////////////////////////////////

Pipeline::Pipeline()
  : new_size_(-1, -1) {}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::set_output_window(WindowPtr const& window) {
  window_ = window;
}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::draw(ConstSerializedScenePtr const& scene) {

  if (!window_->pOpen()) {
    window_->pOpen = true;

    window_->on_resize.connect([&](math::vec2i const& size){
      new_size_ = size;
    });


  }

  if (new_size_ != math::vec2i(-1, -1)) {
    window_->get_context().size = new_size_;
    window_->get_context().gl.Viewport(new_size_.x(), new_size_.y());
    new_size_ = math::vec2i(-1, -1);
  }

  for (auto& object: scene->objects) {
    object.second->draw(window_->get_context());
  }

  window_->display();
}

////////////////////////////////////////////////////////////////////////////////

}
