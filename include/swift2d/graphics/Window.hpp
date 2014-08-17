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
class Window {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Bool Open;
  Bool Fullscreen;
  Bool HideCursor;
  String Title;

  Int   ShadingQuality;
  Bool  SubSampling;
  Float Gamma;

  // ------------------------------------------------------------------- signals
  Signal<>                                    on_close;
  Signal<Key, int, int, int>                  on_key_press;
  Signal<math::vec2i>                         on_resize;
  Signal<math::vec2>                          on_mouse_move;
  Signal<Button, int, int>                    on_mouse_button_press;
  Signal<math::vec2>                          on_mouse_scroll;
  Signal<unsigned>                            on_char;

  Signal<JoystickId, JoystickAxisId, float>   on_joystick_axis_changed;
  Signal<JoystickId, JoystickButtonId>        on_joystick_button_pressed;
  Signal<JoystickId, JoystickButtonId>        on_joystick_button_released;

  // ---------------------------------------------------- construction interface
  Window();
  ~Window();

  static WindowPtr create() {
    return std::make_shared<Window>();
  }

  // ------------------------------------------------------------ public methods
  void process_input();
  void set_active(bool active);
  void display();

  bool key_pressed(Key key) const;
  math::vec2 get_cursor_pos() const;

  RenderContext const& get_context() const { return render_context_; };
  RenderContext& get_context() { return render_context_; };

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void open();
  void close();

  void create_glfw_window();

  void update_joysticks();

  RenderContext render_context_;
  GLFWwindow*   window_;

  std::vector<std::vector<float>> joystick_axis_cache_;
  std::vector<std::vector<int>>   joystick_button_cache_;

  bool vsync_dirty_;
  bool fullscreen_dirty_;

  bool init_glew_;
};

}

#endif  // SWIFT2D_WINDOW_HPP
