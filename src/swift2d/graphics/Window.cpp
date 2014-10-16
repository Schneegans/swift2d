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
#include <swift2d/settings.hpp>

#include <GLFW/glfw3.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Window::Window()
  : Open(false)
  , HideCursor(false)
  , Title("Swift2D")
  , Size(math::vec2i(1024, 768))
  , window_(nullptr)
  , joystick_axis_cache_(static_cast<int>(JoystickId::JOYSTICK_NUM),
                         std::vector<float>(
                         static_cast<int>(JoystickAxisId::JOYSTICK_AXIS_NUM)))
  , joystick_button_cache_(static_cast<int>(JoystickId::JOYSTICK_NUM),
                           std::vector<int>(
                           static_cast<int>(
                           JoystickButtonId::JOYSTICK_BUTTON_NUM)))
  , vsync_dirty_(true)
  , fullscreen_dirty_(false)
  , init_glew_(true)
  , debug_(nullptr)
  , log_sink_(nullptr) {

  Open.on_change().connect([this](bool val) {
    if (val) open();
    else     close();
  });

  SettingsWrapper::get().Settings->VSync.on_change().connect([this](bool) {
    vsync_dirty_ = true;
  });

  SettingsWrapper::get().Settings->Fullscreen.on_change().connect([this](bool) {
    fullscreen_dirty_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

Window::~Window() {
  close();
  glfwDestroyWindow(window_);

  delete debug_;
  delete log_sink_;
}


////////////////////////////////////////////////////////////////////////////////

void Window::process_input() {
  if (window_) {
    glfwPollEvents();
    update_joysticks();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::set_active(bool active) {
  if (window_) {
    glfwMakeContextCurrent(window_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::display() {
  if (vsync_dirty_) {
    vsync_dirty_ = false;
    glfwSwapInterval(SettingsWrapper::get().Settings->VSync() ? 1 : 0);
  }

  if (window_) {
    glfwSwapBuffers(window_);
  }

  // if (fullscreen_dirty_) {
  //   fullscreen_dirty_ = false;

  //   close();
  //   open();
  // }
}

////////////////////////////////////////////////////////////////////////////////

bool Window::key_pressed(Key key) const {
  if (!window_) {
    return false;
  }

  return glfwGetKey(window_, (int)key) == GLFW_PRESS;
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 Window::get_cursor_pos() const {
  if (!window_) {
    return math::vec2(0.f, 0.f);
  }
  double x, y;
  glfwGetCursorPos(window_, &x, &y);
  int height(render_context_.window_size.y());
  return math::vec2(x, height-y);
}

////////////////////////////////////////////////////////////////////////////////

void Window::init_context() {
  if (init_glew_) {
    init_glew_ = false;

    // init glew... seems a bit hacky, but works this way
    glewExperimental = GL_TRUE;
    glewInit(); glGetError();

    debug_ = new ogl::Debug();

    log_sink_ = new ogl::Debug::LogSink([](ogl::Debug::CallbackData const& data) {
      if (data.id != 131217 && data.id != 131184 && data.id != 131185 && data.id != 131204) {
        switch (data.severity) {
        case ogl::Debug::Severity::High:
          LOG_ERROR << "[" << data.id << "] " << data.message << std::endl;
          break;
        case ogl::Debug::Severity::Medium:
          LOG_WARNING << "[" << data.id << "] " << data.message << std::endl;
          break;
        case ogl::Debug::Severity::Low:
          LOG_MESSAGE << "[" << data.id << "] " << data.message << std::endl;
          break;
        case ogl::Debug::Severity::Notification:
          LOG_DEBUG << "[" << data.id << "] " << data.message << std::endl;
          break;
        default:
          LOG_TRACE << "[" << data.id << "] " << data.message << std::endl;
        }
      }
    });

    debug_->MessageControl(
      ogl::Debug::Source::DontCare,
      ogl::Debug::Type::DontCare,
      ogl::Debug::Severity::Low,
      true
    );

    render_context_.gl.Disable(ogl::Capability::DepthTest);
    render_context_.gl.DepthMask(false);
    render_context_.gl.Disable(ogl::Capability::Multisample);

    render_context_.ready = true;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::open() {

  if (!window_) {

    bool fullscreen = SettingsWrapper::get().Settings->Fullscreen();

    if (fullscreen) {
      auto desktop_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
      int desktop_height = desktop_mode->height;
      int desktop_width = desktop_mode->width;

      render_context_.window_size = math::vec2i(desktop_width, desktop_height);
    } else {
      render_context_.window_size = Size();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    window_ = glfwCreateWindow(
      render_context_.window_size.x(), render_context_.window_size.y(),
      Title().c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    WindowManager::get().glfw_windows[window_] = this;

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* w) {
      WindowManager::get().glfw_windows[w]->on_close.emit();
    });

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* w, int width, int height) {
      WindowManager::get().glfw_windows[w]->get_context().window_size = math::vec2i(width, height);
      WindowManager::get().glfw_windows[w]->Size = math::vec2i(width, height);
    });

    glfwSetKeyCallback(window_, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
      WindowManager::get().glfw_windows[w]->on_key_press.emit(static_cast<Key>(key), scancode, action, mods);
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* w, double x, double y) {
      auto window(WindowManager::get().glfw_windows[w]);
      int height(window->get_context().window_size.y());
      window->on_mouse_move.emit(math::vec2(x, height - y));
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* w, int button, int action, int mods) {
      WindowManager::get().glfw_windows[w]->on_mouse_button_press.emit(static_cast<Button>(button), action, mods);
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* w, double x, double y) {
      WindowManager::get().glfw_windows[w]->on_mouse_scroll.emit(math::vec2(x, y));
    });

    glfwSetCharCallback(window_, [](GLFWwindow* w, unsigned c) {
      WindowManager::get().glfw_windows[w]->on_char.emit(c);
    });

    // hide cursor -------------------------------------------------------------
    auto on_hide_cursor_change = [&](bool val) {
      glfwSetInputMode(window_, GLFW_CURSOR, val ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
    };
    on_hide_cursor_change(HideCursor.get());
    HideCursor.on_change().connect(on_hide_cursor_change);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::close() {
  if (window_) {
    glfwDestroyWindow(window_);
    WindowManager::get().glfw_windows.erase(window_);
    window_ = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::update_joysticks() {

  const int joystick_num(static_cast<int>(JoystickId::JOYSTICK_NUM));
  for (int joy(0); joy < joystick_num; ++joy) {
    if (glfwJoystickPresent(joy)) {
      JoystickId joy_id(static_cast<JoystickId>(joy));
      int axes_count(0);
      auto axes_array(glfwGetJoystickAxes(joy, &axes_count));
      for (int axis(0); axis < axes_count; ++axis) {
        float axis_value(axes_array[axis]);
        if (axis_value != joystick_axis_cache_[joy][axis]) {
          JoystickAxisId axis_id(static_cast<JoystickAxisId>(axis));
          on_joystick_axis_changed.emit(joy_id, axis_id, axis_value);
          joystick_axis_cache_[joy][axis] = axis_value;
        }
      }

      int button_count(0);
      auto button_array(glfwGetJoystickButtons(joy, &button_count));
      for (int button(0); button < button_count; ++button) {
        JoystickButtonId button_id(static_cast<JoystickButtonId>(button));
        int button_value(static_cast<int>(button_array[button]));

        if (button_value != joystick_button_cache_[joy][button]) {

          if (button_value == 0) {
            on_joystick_button_released.emit(joy_id, button_id);
          }
          else if (button_value == 1) {
            on_joystick_button_pressed.emit(joy_id, button_id);
          }

          joystick_button_cache_[joy][button] = button_value;
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

}
