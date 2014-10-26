////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_WINDOW_MANAGER_HPP
#define SWIFT2D_WINDOW_MANAGER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/graphics/Window.hpp>

#include <unordered_map>

namespace swift {

// -----------------------------------------------------------------------------
class SWIFT_DLL WindowManager : public Singleton<WindowManager> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  WindowPtr const& current() const { return current_; }

  friend class Singleton<WindowManager>;
  friend class Window;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  WindowManager() : current_(Window::create()) {}
  ~WindowManager() {}

  std::unordered_map<GLFWwindow*, Window*> glfw_windows;
  WindowPtr current_;
};

}

#endif  // SWIFT2D_WINDOW_MANAGER_HPP
