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
  : old_size_(-1, -1) {}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::set_output_window(WindowPtr const& window) {
  window_ = window;
}

////////////////////////////////////////////////////////////////////////////////

void Pipeline::draw(ConstSerializedScenePtr const& scene) {

  // create & update window
  if (!window_->Open()) {
    window_->Open = true;
    compositor_ = Compositor::create();
    compositor_->EnableDynamicLighting = true;
  }

  // update window size
  if (old_size_ != window_->get_context().size) {
    old_size_ = window_->get_context().size;
    compositor_->init(window_->get_context());
    window_->get_context().gl.Viewport(old_size_.x(), old_size_.y());
  }

  // setup projection matrix
  math::mat3 view_matrix(scene->camera->WorldTransform.get());
  math::scale(view_matrix, scene->camera->Size.get());
  window_->get_context().projection_matrix = math::inversed(view_matrix);

  // draw opaque objects
  compositor_->draw_objects(scene, window_->get_context());

  // draw lights
  compositor_->draw_lights(scene, window_->get_context());

  // draw transparent objects
  compositor_->composite(scene, window_->get_context());

  // draw user interface
  compositor_->draw_gui(window_->get_context());

  // finish frame
  window_->display();
}

////////////////////////////////////////////////////////////////////////////////

}
