////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Window.hpp>
#include <swift2d/graphics/WindowManager.hpp>
#include <swift2d/utils/Logger.hpp>

#include <GLFW/glfw3.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Window::Window()
  : pVSync(true)
  , pFullscreen(false) {

  pOpen.on_change().connect([&](bool val) {
    if (val) {
      open_();
    } else {
      close_();
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

Window::~Window() {
  close_();
  glfwDestroyWindow(window_);
}


////////////////////////////////////////////////////////////////////////////////

void Window::process_input() {

  glfwPollEvents();
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

void Window::open_() {

  if (!window_) {

    window_ = glfwCreateWindow(
      640, 480,
      "Hello World",
      pFullscreen.get() ? glfwGetPrimaryMonitor() : nullptr,
      nullptr);

    WindowManager::windows[window_] = this;

    set_active(true);
    glewInit();

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* w) {
      WindowManager::windows[w]->on_close.emit();
    });

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* w, int width, int height) {
      WindowManager::windows[w]->on_resize.emit(math::vec2i(width, height));
    });

    glfwSetKeyCallback(window_, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
      WindowManager::windows[w]->on_key_press.emit(static_cast<Key>(key), scancode, action, mods);
    });

    // apply vsync -------------------------------------------------------------
    auto on_vsync_change = [&](bool val) {
      glfwSwapInterval(val ? 1 : 0);
    };
    on_vsync_change(pVSync.get());
    pVSync.on_change().connect(on_vsync_change);

    // apply fullscreen --------------------------------------------------------
    auto on_fullscreen_change = [&](bool val) {

    };
    pFullscreen.on_change().connect(on_fullscreen_change);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::close_() {
  if (window_) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

}
