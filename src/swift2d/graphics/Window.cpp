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
  : VSync(false)
  , Open(false)
  , Fullscreen(false)
  , window_(nullptr) {

  Open.on_change().connect([&](bool val) {
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
    render_context_.gl.Clear().ColorBuffer();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::open_() {

  if (!window_) {

    render_context_.size = math::vec2i(800, 800);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(
      render_context_.size.x(), render_context_.size.y(),
      "Hello World",
      Fullscreen.get() ? glfwGetPrimaryMonitor() : nullptr,
      nullptr);

    WindowManager::windows[window_] = this;

    set_active(true);

    // init glew... seems a bit hacky, but works this way
    glewExperimental = GL_TRUE;
    glewInit(); glGetError();

    Logger::LOG_DEBUG << "Created OpenGL context with version "
                      << render_context_.gl.MajorVersion()
                      << "." << render_context_.gl.MinorVersion() << std::endl;

    render_context_.gl.Disable(oglplus::Capability::DepthTest);
    render_context_.gl.Enable(oglplus::Capability::Blend);
    render_context_.gl.ClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* w) {
      WindowManager::windows[w]->on_close.emit();
    });

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* w, int width, int height) {
      WindowManager::windows[w]->get_context().size = math::vec2i(width, height);
      WindowManager::windows[w]->on_resize.emit(math::vec2i(width, height));
    });

    glfwSetKeyCallback(window_, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
      WindowManager::windows[w]->on_key_press.emit(static_cast<Key>(key), scancode, action, mods);
    });

    // apply vsync -------------------------------------------------------------
    auto on_vsync_change = [&](bool val) {
      glfwSwapInterval(val ? 1 : 0);
    };
    on_vsync_change(VSync.get());
    VSync.on_change().connect(on_vsync_change);
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
