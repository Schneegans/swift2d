////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/swift2d.hpp>

#include <iostream>

using namespace swift;

int main(int argc, char** argv) {

  // initialize FibreGlass3D
  init(argc, argv);

  // example scene setup -------------------------------------------------------
  auto scene = Scene::create();
  scene->pName.set("main_scene");

  // create a screen
  auto planet = scene->add_object();
  auto sprite = planet->add_component<SpriteComponent>();
  sprite->pTexture.set("diffuse.png");
  sprite->pDepth.set(0.0f);

  // rendering pipeline --------------------------------------------------------
  auto window = Window::create();
  // window->pFullscreen = true;
  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);

  Renderer renderer({pipeline});

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();
  Ticker ticker(1.0 / 60.0);
  ticker.on_tick.connect([&]() {

    sprite->pTransform = math::make_rotate(timer.get_elapsed()*0.1)
                       * math::make_scale(std::sin(timer.get_elapsed())*0.3 + 1);

    renderer.queue_draw({scene});
    window->process_input();
  });

  MainLoop loop;

  window->on_close.connect([&](){
    renderer.stop();
    loop.stop();
  });

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (key == swift::Key::ESCAPE) {
      renderer.stop();
      loop.stop();
    }
  });

  loop.start();

  return 0;
}
