////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Window.hpp>
#include <swift2d/utils/Logger.hpp>

#include <GLFW/glfw3.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Window::~Window() {
  glfwDestroyWindow(window_);
}

////////////////////////////////////////////////////////////////////////////////

void Window::open() {



  if (!window_) {
    window_ = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    set_active(true);

    glewInit();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::process_input() {

  glfwPollEvents();

  // if (window_) {
  //   sf::Event event;
  //   while (window_->pollEvent(event)) {
  //     if (event.type == sf::Event::Closed) {
  //       std::cout << "close" << std::endl;
  //     } else if (event.type == sf::Event::Resized) {
  //       std::cout << "resize" << std::endl;
  //       // on_resize(event.size.width, event.size.height);
  //     }
  //   }
  // }
}

////////////////////////////////////////////////////////////////////////////////

void Window::set_active(bool active) {
  if (window_) {
    glfwMakeContextCurrent(window_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::display() {
  if (window_) {
    glfwSwapBuffers(window_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::on_resize(int width, int height) {
  if (width != render_context_.width || height != render_context_.height) {

    render_context_.width = width;
    render_context_.height = height;

    render_context_.gl.Viewport(width, height);
  }

}

////////////////////////////////////////////////////////////////////////////////

}
