////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/Swift2D.hpp>
#include "SpaceScene.hpp"
#include "Player.hpp"

using namespace swift;

// main ------------------------------------------------------------------------
int main(int argc, char** argv) {

  // initialize Swift2D
  Swift2D::instance()->init(argc, argv);

  // scene ---------------------------------------------------------------------
  auto scene = SpaceScene::create();
  auto camera = scene->add<CameraComponent>();
  camera->Size = math::vec2(2.f, 2.f);

  // player --------------------------------------------------------------------
  // Player::init();
  // Player player(true);

  // rendering pipeline --------------------------------------------------------
  auto window = WindowManager::instance()->get_default();

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();
  Swift2D::instance()->on_frame.connect([&]() {
    double time(timer.get_elapsed());
    timer.reset();

    window->process_input();
    SteamNetwork::instance()->update();
    scene->update(time);
    Swift2D::instance()->display(scene, camera);
  });

  window->on_close.connect([&](){
    Swift2D::instance()->stop();
  });

  window->on_key_press.connect([&](Key key, int scancode, int action, int mods) {
    if (action != 1) {
      switch(key) {
        case Key::ESCAPE:
          Swift2D::instance()->stop();
          break;
      }
    }
  });

  Swift2D::instance()->start();
  Swift2D::instance()->clean_up();

  return 0;
}
