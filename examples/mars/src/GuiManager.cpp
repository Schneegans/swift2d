////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/GuiManager.hpp"

using namespace swift;

////////////////////////////////////////////////////////////////////////////////

GuiManager::GuiManager() {

  callbacks_["quit"] = [&](){
    on_quit.emit();
  };

  callbacks_["hide_quit"] = [&](){
    quit_->hide();
  };
  callbacks_["show_quit"] = [&](){
    quit_->show();
  };
  callbacks_["toggle_quit"] = [&](){
    quit_->toggle();
  };

  callbacks_["show_options"] = [&](){
    options_->show();
  };
  callbacks_["hide_options"] = [&](){
    options_->hide();
  };
  callbacks_["toggle_options"] = [&](){
    options_->toggle();
  };

  auto scene = SceneManager::instance()->get_default();

  menu_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, false));
  menu_->Resource = Application::instance()->get_resource("gui", "main_menu.html");
  menu_->Size = math::vec2i(525, 180);
  menu_->Anchor = math::vec2i(-1, 1);

  options_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, true));
  options_->Resource = Application::instance()->get_resource("gui", "options.html");
  options_->Size = math::vec2i(1000, 1000);
  options_->Anchor = math::vec2i(0, 1);

  quit_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, true));
  quit_->Resource = Application::instance()->get_resource("gui", "quit.html");
  quit_->Size = math::vec2i(700, 700);
  quit_->Anchor = math::vec2i(0, 1);

  fps_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, false));
  fps_->Resource = Application::instance()->get_resource("gui", "fps.html");
  fps_->Size = math::vec2i(240, 35);
  fps_->Anchor = math::vec2i(-1, -1);

  mouse_ = scene->add(std::make_shared<MarsGuiComponent>(callbacks_, false));
  mouse_->Resource = Application::instance()->get_resource("gui", "mouse.html");
  mouse_->Size = math::vec2i(50, 50);
  mouse_->Anchor = math::vec2i(-1, -1);

  Interface::instance()->on_cursor_change.connect([&](Cursor pointer){
    mouse_->call_javascript("set_active", std::to_string(pointer == Cursor::HAND));
  });

  auto window = WindowManager::instance()->get_default();
  window->on_mouse_move.connect([&](math::vec2 const& pos){
    mouse_->Offset = pos + math::vec2(-5.f, -45.f);
  });

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (key == swift::Key::ESCAPE && action != 1) {
      quit_->toggle();
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
  std::stringstream sstr;
  sstr.precision(1);
  sstr.setf(std::ios::fixed, std::ios::floatfield);
  sstr << "FPS: " << app_fps << " / " << render_fps;
  fps_->call_javascript("set_fps_text", sstr.str());

  Interface::instance()->update();
}

////////////////////////////////////////////////////////////////////////////////
