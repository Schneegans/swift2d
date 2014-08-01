////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/GuiManager.hpp"
#include "../include/MusicManager.hpp"

using namespace swift;

////////////////////////////////////////////////////////////////////////////////

GuiManager::GuiManager()
  : frame_count_(0) {

  callbacks_["quit"] = [&](){
    on_quit.emit();
  };

  callbacks_["hide_quit"] = [&](){
    hide_window(quit_);
  };
  callbacks_["show_quit"] = [&](){
    show_window(quit_);
  };
  callbacks_["toggle_quit"] = [&](){
    toggle_window(quit_);
  };

  callbacks_["show_options"] = [&](){
    show_window(options_);
  };
  callbacks_["hide_options"] = [&](){
    hide_window(options_);
  };
  callbacks_["toggle_options"] = [&](){
    toggle_window(options_);
  };

  callbacks_["audio_next"] = [&](){
    MusicManager::instance()->play_next();
  };
  callbacks_["audio_prev"] = [&](){
    MusicManager::instance()->play_prev();
  };
  callbacks_["audio_pause"] = [&](){
    MusicManager::instance()->pause();
  };

  auto scene = SceneManager::instance()->get_default();

  mouse_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, false));
  mouse_->Resource = Application::instance()->get_resource("gui", "mouse.html");
  mouse_->Size = math::vec2i(50, 50);
  mouse_->Anchor = math::vec2i(-1, -1);
  mouse_->Depth = 100;

  Interface::instance()->on_cursor_change.connect([&](Cursor pointer){
    mouse_->call_javascript("set_active", std::to_string(pointer == Cursor::HAND));
  });

  auto window = WindowManager::instance()->get_default();
  window->on_mouse_move.connect([&](math::vec2 const& pos){
    mouse_->Offset = pos + math::vec2(-5.f, -45.f);
  });

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (key == swift::Key::ESCAPE && action != 1) {
      toggle_window(quit_);
    } else if (key == swift::Key::F5 && action != 1) {
      menu_->reload();
      options_->reload();
      quit_->reload();
      fps_->reload();
      mouse_->reload();
    }
  });

}

////////////////////////////////////////////////////////////////////////////////

void GuiManager::update(float app_fps, float render_fps) {

  auto scene = SceneManager::instance()->get_default();
  ++frame_count_;

  if (frame_count_ == 1) {
    fps_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, false));
    fps_->Resource = Application::instance()->get_resource("gui", "fps.html");
    fps_->Size = math::vec2i(200, 55);
    fps_->Anchor = math::vec2i(0, -1);
    fps_->Depth = 0;
  }

  if (frame_count_ == 2) {
    player_left_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, false));
    player_left_->Resource = Application::instance()->get_resource("gui", "player_left.html");
    player_left_->Size = math::vec2i(470, 230);
    player_left_->Anchor = math::vec2i(-1, -1);
    player_left_->Depth = 0;
  }

  if (frame_count_ == 3) {
    quit_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, true));
    quit_->Resource = Application::instance()->get_resource("gui", "quit.html");
    quit_->Size = math::vec2i(700, 700);
    quit_->Anchor = math::vec2i(0, 1);
  }

  if (frame_count_ == 4) {
    options_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, true));
    options_->Resource = Application::instance()->get_resource("gui", "options.html");
    options_->Size = math::vec2i(1000, 1000);
    options_->Anchor = math::vec2i(0, 1);
  }

  if (frame_count_ == 5) {
    music_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, false));
    music_->Resource = Application::instance()->get_resource("gui", "music_menu.html");
    music_->Size = math::vec2i(520, 175);
    music_->Anchor = math::vec2i(1, 1);
    music_->Depth = 0;
  }

  if (frame_count_ == 6) {
    menu_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, false));
    menu_->Resource = Application::instance()->get_resource("gui", "main_menu.html");
    menu_->Size = math::vec2i(525, 180);
    menu_->Anchor = math::vec2i(-1, 1);
    menu_->Depth = 0;
  }


  std::stringstream sstr;
  sstr.precision(1);
  sstr.setf(std::ios::fixed, std::ios::floatfield);
  sstr << "FPS: " << app_fps << " / " << render_fps;
  fps_->call_javascript("set_fps_text", sstr.str());

  Interface::instance()->update();
}

////////////////////////////////////////////////////////////////////////////////

void GuiManager::show_track_info(std::string const& title, std::string const& artist, std::string const& album) {
  music_->call_javascript("set_music_info", {title, artist, album});
}

////////////////////////////////////////////////////////////////////////////////

void GuiManager::show_window(MarsGuiComponentPtr& window) {
  window->show();
  if (open_windows_.size() > 0) {
    open_windows_.back()->Active = false;
    window->Depth = open_windows_.back()->Depth() + 1;
  } else {
    window->Depth = 1;
  }
  window->Active = true;
  open_windows_.push_back(window);
}

////////////////////////////////////////////////////////////////////////////////

void GuiManager::hide_window(MarsGuiComponentPtr& window) {
  window->hide();
  for (auto it(open_windows_.begin()); it != open_windows_.end(); ++it) {
    if (*it == window) {
      open_windows_.erase(it);
      break;
    }
  }
  if (open_windows_.size() > 0) {
    open_windows_.back()->Active = true;
  }
}

////////////////////////////////////////////////////////////////////////////////

void GuiManager::toggle_window(MarsGuiComponentPtr& window) {
  if (window->is_visible()) hide_window(window);
  else                      show_window(window);
}


////////////////////////////////////////////////////////////////////////////////
