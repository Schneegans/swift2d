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
#include <swift2d/input/keys.hpp>

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
  Bool VSync;
  Bool Fullscreen;

  // ------------------------------------------------------------------- signals
  Signal<>                    on_close;
  Signal<Key, int, int, int>  on_key_press;
  Signal<math::vec2i>         on_resize;

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

  RenderContext const& get_context() const { return render_context_; };
  RenderContext& get_context() { return render_context_; };

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

    void open_();
    void close_();

    RenderContext render_context_;
    GLFWwindow* window_;
};

}

#endif  // SWIFT2D_WINDOW_HPP
