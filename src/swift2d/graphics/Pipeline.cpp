////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Pipeline.hpp>

#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/components/CameraComponent.hpp>
#include <swift2d/graphics/Window.hpp>
#include <swift2d/utils/Logger.hpp>
#include <thread>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>


namespace swift {

////////////////////////////////////////////////////////////////////////////////

Pipeline::Pipeline()
  : old_size_(-1, -1)
  , compositor_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::set_output_window(WindowPtr const& window) {
  window_ = window;
}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::draw(ConstSerializedScenePtr const& scene) {

  // create & update window
  if (!window_->Open()) {
    window_->Open = true;
  }

  // update window size
  if (old_size_ != window_->get_context().size) {
    old_size_ = window_->get_context().size;

    if (compositor_) {
      delete compositor_;
      compositor_ = nullptr;
    }

    compositor_ = new Compositor(window_->get_context(), 5);
  }

  window_->get_context().gl.Viewport(old_size_.x(), old_size_.y());

  // setup projection matrix
  math::mat3 view_matrix(scene->camera->WorldTransform.get());
  math::scale(view_matrix, scene->camera->Size.get());
  window_->get_context().projection_matrix = math::inversed(view_matrix);
  window_->get_context().projection_parallax = scene->camera->Parallax();

  // draw opaque objects
  compositor_->draw_objects(scene, window_->get_context());

  // draw lights
  compositor_->draw_lights(scene, window_->get_context());

  // perform post processing
  compositor_->post_process(scene, window_->get_context());

  // draw user interface
  compositor_->draw_gui(scene, window_->get_context());

  // finish frame
  window_->display();
}

////////////////////////////////////////////////////////////////////////////////

}
