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
#include <swift2d/graphics/Window.hpp>

#include <unordered_map>

namespace swift {

// -----------------------------------------------------------------------------
class WindowManager {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  static std::unordered_map<GLFWwindow*, Window*> windows;

};

}

#endif  // SWIFT2D_WINDOW_MANAGER_HPP
