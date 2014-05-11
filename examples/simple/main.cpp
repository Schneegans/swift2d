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

  // initialize Swift2D
  init(argc, argv);

  MainLoop loop;

  auto window = Window::create();
  // window->pFullscreen = true;


  class Mover: public MoveBehavior {
   public:
    Mover() {}
    Mover(WindowPtr const& w) {
      w->on_key_press.connect([&](Key key, int scancode, int action, int mods){
        if (key == Key::W) {
          pLinearSpeed = action != 0 ? 10 : 0;
        }

        if (key == Key::A) {
          pAngularSpeed = action != 0 ? -1 : 0;
        }

        if (key == Key::D) {
          pAngularSpeed = action != 0 ? 1 : 0;
        }
      });
    }
  };





  // example scene setup -------------------------------------------------------
  auto scene = SceneObject::create();

  auto bg = scene->add<SpriteComponent>();
       bg->pDepth = -1000.0f;
       bg->sprite_ = new SpriteResource();
       bg->tex_ = new TextureResource("bg.png");
       bg->pTransform = math::make_scale(2.f);

  auto camera = scene->add<CameraComponent>();
  camera->pSize = math::vec2(2.f, 2.f);

  // planet
  auto planet = scene->add();

  auto sprite = planet->add<SpriteComponent>();
       sprite->pDepth = 0.0f;
       sprite->sprite_ = bg->sprite_;
       sprite->tex_ = new TextureResource("diffuse.png");

  auto boing = planet->add<SoundComponent>();
       boing->set_sound(new SoundResource("sound.wav"));

  // player
  auto player = scene->add();
       player->pTransform = math::make_scale(0.1);

  auto mover = std::make_shared<Mover>(window);
  player->add(mover);

  auto listener = player->add<ListenerComponent>();
       listener->pVolume = 10.0;

  auto ship = player->add<SpriteComponent>();
       ship->pDepth = 1.0f;
       ship->sprite_ = bg->sprite_;
       ship->tex_ = new TextureResource("ship.png");

  // rendering pipeline --------------------------------------------------------

  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);

  Renderer renderer(pipeline);

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();

  Ticker ticker(1.0 / 60.0);
  ticker.on_tick.connect([&]() {
    renderer.process(scene, camera, timer.get_elapsed());
    timer.reset();
    window->process_input();
  });

  window->on_close.connect([&](){
    renderer.stop();
    loop.stop();
  });

  window->on_resize.connect([&](math::vec2i const& size){
    float height = 2;
    float aspect = 1.0f * size.x() / size.y();
    math::vec2 camera_size(height*aspect, height);
    camera->pSize = camera_size;
  });

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (key == swift::Key::ESCAPE) {
      renderer.stop();
      loop.stop();
    } else if (key == swift::Key::SPACE && action == 0) {
      boing->play();
    }
  });

  loop.start();

  return 0;
}
