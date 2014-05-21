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

class Mover: public MoveBehavior {
 public:
  Mover() {}
  Mover(WindowPtr const& w) {
    w->on_key_press.connect([&](Key key, int scancode, int action, int mods){
      if (action == 0) {
        if (key == Key::W) LinearSpeed.set(0, 0.5);
        if (key == Key::S) LinearSpeed.set(0, 0.5);
        if (key == Key::A) AngularSpeed.set(0, 0.1);
        if (key == Key::D) AngularSpeed.set(0, 0.1);
      } else if (action == 1) {
        if (key == Key::W) LinearSpeed.set( 20, 1);
        if (key == Key::S) LinearSpeed.set(-20, 1);
        if (key == Key::A) AngularSpeed.set(-2 , 0.5);
        if (key == Key::D) AngularSpeed.set( 2 , 0.5);
      }
    });
  }
};



class Bullet: public SceneObject {
 public:
  Bullet(SceneObjectPtr const& scene) {

    auto move = add<MoveBehavior>();
         move->LinearSpeed.set(10);

    auto tex = add<SpriteComponent>();
         tex->Depth = 10.0f;
         tex->Material = MaterialDatabase::instance()->get("bullet");
         tex->Transform = math::make_scale(1.5f);

    auto light = add<PointLightComponent>();
         light->Depth = 1.0f;
         light->Transform = math::make_scale(5.0f);
         light->Material = MaterialDatabase::instance()->get("light");

    auto shape = add<CircularShape>();

    auto deleter = add<DeleteOnLeaveBehavior>();
         deleter->set_shapes(shape, scene->get_component<CircularShape>());
  }
};



int main(int argc, char** argv) {

  // initialize Swift2D
  init(argc, argv);

  MainLoop loop;

  // load resources ------------------------------------------------------------
  MaterialDatabase::instance()->add("background", ShadelessTextureMaterial::create_from_file("bg.jpg"));
  MaterialDatabase::instance()->add("ship",       ShadelessTextureMaterial::create_from_file("ship.png"));
  MaterialDatabase::instance()->add("bullet",     ShadelessTextureMaterial::create_from_file("bullet.png"));

  MaterialDatabase::instance()->add("planet",     BumpTextureMaterial::create_from_files("diffuse.png", "normal.png"));

  MaterialDatabase::instance()->add("light",      LightMaterial::create_from_file("light.png"));


  // window setup --------------------------------------------------------------
  auto window = Window::create();
  // window->Fullscreen = true;

  // example scene setup -------------------------------------------------------
  auto scene = SceneObject::create();
  auto music = scene->add<SoundComponent>();
       music->Sound = Sound::create_from_file("music.ogg");
       music->Volume = 0.1f;
       // music->play();

  auto field = scene->add<CircularShape>();
       field->Transform = math::make_scale(2);

  auto sun = scene->add<PointLightComponent>();
       sun->Transform = math::make_scale(5) * math::make_translate(-0.5, 0.5);
       sun->Material = MaterialDatabase::instance()->get("light");

  auto bg = scene->add<SpriteComponent>();
       bg->Depth = -1000.0f;
       bg->Material = MaterialDatabase::instance()->get("background");
       bg->Transform = math::make_scale(2.f);


  auto camera = scene->add<CameraComponent>();
  camera->Size = math::vec2(2.f, 2.f);

  // planet
  auto planet = scene->add_object();

  auto sprite = planet->add<SpriteComponent>();
       sprite->Depth = 0.0f;
       sprite->Material = MaterialDatabase::instance()->get("planet");

  auto boing = planet->add<SoundComponent>();
       boing->Sound = Sound::create_from_file("sound.wav");

  // player
  auto player = scene->add_object();
       player->Transform = math::make_scale(0.1);

  auto mover = std::make_shared<Mover>(window);
  player->add(mover);

  auto listener = player->add<ListenerComponent>();
       listener->Volume = 1.0;

  auto ship = player->add<SpriteComponent>();
       ship->Depth = 1.0f;
       ship->Material = MaterialDatabase::instance()->get("ship");

  auto light = player->add<PointLightComponent>();
       light->Depth = 1.0f;
       light->Transform = math::make_scale(10);
       light->Material = MaterialDatabase::instance()->get("light");

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
    camera->Size = camera_size;
  });

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (key == swift::Key::ESCAPE) {
      renderer.stop();
      loop.stop();
    } else if (key == swift::Key::SPACE && action != 1) {
      // boing->play();

      auto bullet = std::make_shared<Bullet>(scene);
      scene->add_object(bullet);
      bullet->Transform = player->Transform();
    }
  });

  loop.start();

  return 0;
}
