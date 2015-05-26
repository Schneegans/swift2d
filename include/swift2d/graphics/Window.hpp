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

#ifndef SWIFT2D_WINDOW_HPP
#define SWIFT2D_WINDOW_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/properties.hpp>
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/input/keyboard_enums.hpp>
#include <swift2d/input/mouse_enums.hpp>
#include <swift2d/input/joystick_enums.hpp>

#include <memory>

// forward declares ------------------------------------------------------------
class GLFWwindow;

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Window;
typedef std::shared_ptr<Window>       WindowPtr;
typedef std::shared_ptr<const Window> ConstWindowPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL Window {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  enum class Mode {
    WINDOWED,
    WINDOWED_FULLSCREEN,
    FULLSCREEN
  };

  // ---------------------------------------------------------------- properties
  Bool   Minimized;
  Bool   HideCursor;
  String Title;

  // ------------------------------------------------------------------- signals
  Signal<>                                    on_close;

  Signal<Key, int, int, int>                  on_key_press;
  Signal<unsigned>                            on_char;
  Signal<math::vec2>                          on_mouse_move;
  Signal<Button, int, int>                    on_mouse_button_press;
  Signal<math::vec2>                          on_mouse_scroll;

  Signal<JoystickId, JoystickAxisId, float>   on_joystick_axis_changed;
  Signal<JoystickId, JoystickButtonId>        on_joystick_button_pressed;
  Signal<JoystickId, JoystickButtonId>        on_joystick_button_released;

  // ---------------------------------------------------- construction interface
  Window(bool debug);
  ~Window();

  static WindowPtr create(bool debug) {
    return std::make_shared<Window>(debug);
  }

  // ------------------------------------------------------------ public methods
  void open();
  void close();

  void process_input();
  void set_active(bool active);
  void display();

  bool key_pressed(Key key) const;
  float joy_axis(int joy_stick, int axis);
  math::vec2 get_cursor_pos() const;

  RenderContext const& get_context() const { return render_context_; };
  RenderContext&       get_context()       { return render_context_; };

  void update_context();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  void update_joysticks();

  RenderContext render_context_;
  GLFWwindow*   window_;

  std::vector<std::vector<float>> joystick_axis_cache_;
  std::vector<std::vector<int>>   joystick_button_cache_;

  bool vsync_dirty_;
  bool mode_dirty_;
  bool size_dirty_;
  bool init_glew_;
  bool debug_;

  math::vec2ui size_;

  // oglplus::Debug*           debugger_;
  // oglplus::Debug::LogSink*  log_sink_;
};

// -----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, Window::Mode const& obj);
std::istream& operator>>(std::istream& is, Window::Mode& obj);

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_WINDOW_HPP
