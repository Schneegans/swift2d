////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Window.hpp>

#include <swift2d/opengl.hpp>
#include <swift2d/graphics/WindowManager.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/settings.hpp>

#include <GLFW/glfw3.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Window::Window(bool debug)
  : Minimized(false)
  , HideCursor(false)
  , Title("Swift2D")
  , window_(nullptr)
  , joystick_axis_cache_(static_cast<int>(JoystickId::JOYSTICK_NUM),
                         std::vector<float>(
                         static_cast<int>(JoystickAxisId::JOYSTICK_AXIS_NUM)))
  , joystick_button_cache_(static_cast<int>(JoystickId::JOYSTICK_NUM),
                           std::vector<int>(
                           static_cast<int>(
                           JoystickButtonId::JOYSTICK_BUTTON_NUM)))
  , vsync_dirty_(true)
  , mode_dirty_(false)
  , size_dirty_(false)
  , init_glew_(true)
  // , debugger_(nullptr)
  // , log_sink_(nullptr)
  , debug_(debug)
  , size_(math::vec2ui(1024, 768)) {

  SettingsWrapper::get().Settings->VSync.on_change().connect([this](bool) {
    vsync_dirty_ = true;
    return true;
  });

  SettingsWrapper::get().Settings->WindowMode.on_change().connect([this](Mode) {
    mode_dirty_ = true;
    return true;
  });

  SettingsWrapper::get().Settings->WindowSize.on_change().connect([this](math::vec2i const& val) {
    if (val.x() > 50 && val.y() > 50) {
      Minimized = false;
    } else {
      Minimized = true;
    }
    size_dirty_ = true;
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

Window::~Window() {
  close();
  glfwDestroyWindow(window_);

  //delete debugger_;
  //delete log_sink_;
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

float Window::joy_axis(int joy_stick, int axis) {
  if (joy_stick >= JOYSTICK_NUM || axis >= JOYSTICK_AXIS_NUM) {
    return 0;
  }
  if (!glfwJoystickPresent(joy_stick)) {
    return 0;
  }
  return joystick_axis_cache_[joy_stick][axis];
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

void Window::update_context() {
  if (init_glew_) {
    init_glew_ = false;

    // glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

    // init glew... seems a bit hacky, but works this way
    glewExperimental = GL_TRUE;
    auto e = glewInit();
    if (e != GLEW_OK) {
      LOG_ERROR << "Failed to initialize glew: " << glewGetErrorString(e) << std::endl;
    } else {
      int major = glfwGetWindowAttrib(window_, GLFW_CONTEXT_VERSION_MAJOR);
      int minor = glfwGetWindowAttrib(window_, GLFW_CONTEXT_VERSION_MINOR);
      int rev   = glfwGetWindowAttrib(window_, GLFW_CONTEXT_REVISION);
      LOG_MESSAGE << "Initialized OpenGL context " << major << "." << minor
                  << "." << rev << " successfully." << std::endl;
    }

    glGetError();

    if (debug_) {
      // debugger_ = new ogl::Debug();

      // log_sink_ = new ogl::Debug::LogSink([](ogl::Debug::CallbackData const& data) {
      //   if (data.id != 131185 && data.id != 131204 && data.id != 131184 && data.id != 131076) {
      //     switch (data.severity) {
      //     case ogl::Debug::Severity::High:
      //       LOG_ERROR << "[" << data.id << "] " << data.message << std::endl;
      //       break;
      //     case ogl::Debug::Severity::Medium:
      //       LOG_WARNING << "[" << data.id << "] " << data.message << std::endl;
      //       break;
      //     case ogl::Debug::Severity::Low:
      //       LOG_MESSAGE << "[" << data.id << "] " << data.message << std::endl;
      //       break;
      //     case ogl::Debug::Severity::Notification:
      //       LOG_DEBUG << "[" << data.id << "] " << data.message << std::endl;
      //       break;
      //     default:
      //       LOG_TRACE << "[" << data.id << "] " << data.message << std::endl;
      //     }
      //   }
      // });

      // debugger_->MessageControl(
      //   ogl::Debug::Source::DontCare,
      //   ogl::Debug::Type::DontCare,
      //   ogl::Debug::Severity::Low,
      //   true
      // );
    }

    ogl::Context::Disable(ogl::Capability::DepthTest);
    ogl::Context::DepthMask(false);
    ogl::Context::Disable(ogl::Capability::Multisample);

    render_context_.ready = true;
  }

  if (size_dirty_) {
    size_dirty_ = false;
    size_ = SettingsWrapper::get().Settings->WindowSize();
  }

  render_context_.window_size         = size_;
  render_context_.sub_sampling        = SettingsWrapper::get().Settings->SubSampling();
  render_context_.dynamic_lighting    = SettingsWrapper::get().Settings->DynamicLighting();
  render_context_.light_sub_sampling  = SettingsWrapper::get().Settings->LightSubSampling();
  render_context_.lens_flares         = SettingsWrapper::get().Settings->LensFlares();
  render_context_.heat_effect         = SettingsWrapper::get().Settings->HeatEffect();
}

////////////////////////////////////////////////////////////////////////////////

void Window::open() {

  if (!window_) {

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    if (debug_) {
      glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }

    auto desktop_mode  = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int desktop_height = desktop_mode->height;
    int desktop_width  = desktop_mode->width;

    switch(SettingsWrapper::get().Settings->WindowMode()) {
      case Mode::WINDOWED:
        size_ = SettingsWrapper::get().Settings->WindowSize();
        window_ = glfwCreateWindow(
          size_.x(), size_.y(), Title().c_str(), nullptr, nullptr);
        break;
      case Mode::WINDOWED_FULLSCREEN:
        size_ = math::vec2i(desktop_width, desktop_height);
        glfwWindowHint(GLFW_RED_BITS, desktop_mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, desktop_mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, desktop_mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, desktop_mode->refreshRate);
        window_ = glfwCreateWindow(
          size_.x(), size_.y(), Title().c_str(), glfwGetPrimaryMonitor(), nullptr);
        break;
      case Mode::FULLSCREEN:
        size_ = math::vec2i(desktop_width, desktop_height);
        window_ = glfwCreateWindow(
          size_.x(), size_.y(), Title().c_str(), glfwGetPrimaryMonitor(), nullptr);
        break;
    }

    SettingsWrapper::get().Settings->WindowSize = size_;
    render_context_.window_size = size_;

    WindowManager::get().glfw_windows[window_] = this;

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* w) {
      WindowManager::get().glfw_windows[w]->on_close.emit();
    });

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* w, int width, int height) {
      SettingsWrapper::get().Settings->WindowSize = math::vec2i(width, height);
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
      return true;
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

  float changed_threshold(0.01f);
  const float min_threshold(0.15f);
  const float max_threshold(0.9f);

  const int joystick_num(static_cast<int>(JoystickId::JOYSTICK_NUM));
  for (int joy(0); joy < joystick_num; ++joy) {
    if (glfwJoystickPresent(joy)) {
      JoystickId joy_id(static_cast<JoystickId>(joy));
      int axes_count(0);
      auto axes_array(glfwGetJoystickAxes(joy, &axes_count));
      
      for (int axis(0); axis < axes_count; ++axis) {
        float axis_value(axes_array[axis]);

        // XBOX controller left and right trigger are both mapped to axis 2 on
        // windows.
        #if defined( _WIN32 )
          if (axis == 2) {
            if (axis_value < 0) {
              axis = 5;
              axis_value = -axis_value;
            }
          }
        #else
          if (axis == 2 || axis == 5) {
            axis_value = (axis_value + 1)*0.5;
          }
        #endif

        int sign(axis_value < 0.f ? -1 : 1);
        axis_value = std::abs(axis_value);

        axis_value = (axis_value - min_threshold) / (max_threshold - min_threshold);
        axis_value = sign * std::min(1.f, std::max(0.f, axis_value));

        if (axis_value == 0.f || axis_value == 1.f || axis_value == -1.f) {
          changed_threshold = 0.f;
        }

        if (std::abs(axis_value - joystick_axis_cache_[joy][axis]) > changed_threshold) {
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

std::ostream& operator<<(std::ostream& os, Window::Mode const& obj) {
  os << static_cast<int>(obj);
  return os;
}

////////////////////////////////////////////////////////////////////////////////

std::istream& operator>>(std::istream& is, Window::Mode& obj) {
  int tmp(0);
  is >> tmp;
  obj = static_cast<Window::Mode>(tmp);
  return is;
}

////////////////////////////////////////////////////////////////////////////////

}
