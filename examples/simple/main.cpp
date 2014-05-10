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

  MainLoop loop;

  // example scene setup -------------------------------------------------------
  auto scene = Scene::create();
  scene->pName.set("main_scene");

  auto planet = scene->add_object();
  auto sprite = planet->add<SpriteComponent>();
       sprite->pDepth.set(0.0f);
       sprite->sprite_ = new SpriteResource();
       sprite->tex_ = new TextureResource("diffuse.png");
  auto boing = planet->add<SoundComponent>();
       boing->set_sound(new SoundResource("sound.wav"));

  auto player = scene->add_object();
       player->pTransform = math::make_scale(0.1);
  auto listener = player->add<ListenerComponent>();
       listener->pVolume = 10.0;
  auto ship = player->add<SpriteComponent>();
       ship->pDepth.set(1.0f);
       ship->sprite_ = sprite->sprite_;
       ship->tex_ = new TextureResource("icon.png");

  // rendering pipeline --------------------------------------------------------
  auto window = Window::create();
  // window->pFullscreen = true;
  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);

  Renderer graphics({pipeline});

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();
  Ticker ticker(1.0 / 90.0);
  ticker.on_tick.connect([&]() {

    planet->pTransform = math::make_rotate(timer.get_elapsed()*0.3)
                       * math::make_scale(std::sin(timer.get_elapsed())*0.1 + 0.3)
                       * math::make_translate(std::sin(timer.get_elapsed())*3, 0);

    graphics.process({scene});
    window->process_input();
  });

  window->on_close.connect([&](){
    graphics.stop();
    loop.stop();
  });

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (key == swift::Key::ESCAPE) {
      graphics.stop();
      loop.stop();
    } else if (key == swift::Key::SPACE && action == 0) {
      boing->play();
    }
  });

  loop.start();

  return 0;
}
