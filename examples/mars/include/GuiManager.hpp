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

class GuiManager : public swift::Singleton<GuiManager> {
 public:

  swift::Signal<> on_quit;

  void update(float app_fps, float render_fps);

  void show_track_info(std::string const& title,
                       std::string const& artist,
                       std::string const& album);

  friend class swift::Singleton<GuiManager>;

 private:

  GuiManager();
  ~GuiManager() {};

  void show_window(MarsGuiComponentPtr& window);
  void hide_window(MarsGuiComponentPtr& window);
  void toggle_window(MarsGuiComponentPtr& window);

  MarsGuiComponentPtr menu_;
  MarsGuiComponentPtr music_;
  MarsGuiComponentPtr options_;
  MarsGuiComponentPtr quit_;
  MarsGuiComponentPtr fps_;
  MarsGuiComponentPtr mouse_;
  MarsGuiComponentPtr player_left_;

  std::unordered_map<std::string, std::function<void()>> callbacks_;
  std::vector<MarsGuiComponentPtr> open_windows_;

};

// -----------------------------------------------------------------------------

#endif // MARS_GUI_MANAGER_HPP
