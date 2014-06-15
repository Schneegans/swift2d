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
  Application app(argc, argv);
  Network::instance()->connect("myTestGame");

  // scene ---------------------------------------------------------------------
  auto scene = SpaceScene::create(app);
  auto camera = scene->add<CameraComponent>();
  camera->Size = math::vec2(2.f, 2.f);

  // player --------------------------------------------------------------------
  Player::init();

  Player player(true);
  player.distribute();

  // rendering pipeline --------------------------------------------------------
  auto window = WindowManager::instance()->get_default();
  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);

  Renderer renderer;
  renderer.set_pipeline(pipeline);

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();

  auto ticker(Ticker::create(1.0 / 60.0));
  ticker->on_tick.connect([&]() {
    double time(timer.get_elapsed());
    timer.reset();

    window->process_input();
    Network::instance()->update();
    scene->update(time);
    renderer.process(scene, camera);
  });

  ticker->start();

  window->on_close.connect([&](){
    renderer.stop();
    app.stop();
  });

  window->on_key_press.connect([&](Key key, int scancode, int action, int mods) {
    if (action != 1) {
      switch(key) {
        case Key::ESCAPE:
          renderer.stop();
          app.stop();
          break;
      }
    }
  });

  app.start();

  Network::instance()->disconnect();

  return 0;
}

