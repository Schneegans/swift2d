////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/swift2d.hpp>
#include "SpaceScene.hpp"
#include "Player.hpp"

using namespace swift;

// main ------------------------------------------------------------------------
int main(int argc, char** argv) {

  // initialize Swift2D
  Application::get().init(argc, argv);
  Network::get().connect("TestGame");

  // scene ---------------------------------------------------------------------
  auto scene = SpaceScene::create();
  auto camera = scene->add<CameraComponent>();
  camera->Size = math::vec2(2.f, 2.f);

  // player --------------------------------------------------------------------
  Player::init();
  Player player(true);

  // rendering pipeline --------------------------------------------------------
  auto window = WindowManager::get().get_default();

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();
  Application::get().on_frame.connect([&]() {
    double time(timer.get_elapsed());
    timer.reset();

    window->process_input();
    Network::get().update();
    scene->update(time);
    Application::get().display(scene, camera);
  });

  window->on_close.connect([&](){
    Application::get().stop();
  });

  window->on_key_press.connect([&](Key key, int scancode, int action, int mods) {
    if (action != 1) {
      switch(key) {
        case Key::ESCAPE:
          Application::get().stop();
          break;
      }
    }
  });

  Application::get().start();
  Application::get().clean_up();

  return 0;
}
