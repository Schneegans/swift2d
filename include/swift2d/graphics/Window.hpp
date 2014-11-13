////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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

  // ---------------------------------------------------------------- properties
  Bool   Minimized;
  Bool   HideCursor;
  String Title;

  // ------------------------------------------------------------------- signals
  Signal<>                                    on_close;
  Signal<math::vec2i>                         on_size_change;

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

  void set_size(math::vec2i const& size);
  math::vec2i const& get_size() const;

  bool key_pressed(Key key) const;
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
  bool fullscreen_dirty_;
  bool init_glew_;
  bool debug_;

  math::vec2i size_;

  //ogl::Debug*           debugger_;
  //ogl::Debug::LogSink*  log_sink_;
};

}

#endif  // SWIFT2D_WINDOW_HPP
