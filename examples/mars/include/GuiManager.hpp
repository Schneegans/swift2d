////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_GUI_MANAGER_HPP
#define MARS_GUI_MANAGER_HPP

// includes  -------------------------------------------------------------------
#include "../include/MarsGuiComponent.hpp"

class GuiManager {
 public:

  swift::Signal<> on_quit;

  GuiManager();

  void update(float app_fps, float render_fps);

 private:
  MarsGuiComponentPtr menu_;
  MarsGuiComponentPtr options_;
  MarsGuiComponentPtr quit_;
  MarsGuiComponentPtr fps_;
  MarsGuiComponentPtr mouse_;

  std::unordered_map<std::string, std::function<void()>> callbacks_;

};

// -----------------------------------------------------------------------------

#endif // MARS_GUI_MANAGER_HPP
