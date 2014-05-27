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

int particle_count(0);

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
         tex->Transform = math::make_scale(1.0f);

    auto light = add<LightComponent>();
         light->Depth = 1.0f;
         light->Transform = math::make_scale(5.0f);
         light->Material = MaterialDatabase::instance()->get("light");

    auto shape = add<CircularShape>();

    auto deleter = add<DeleteOnLeaveBehavior>();
         deleter->set_shapes(shape, scene->get_component<CircularShape>());
         deleter->on_delete.connect([&](){ --particle_count; });
  }
};

class Spark: public SceneObject {
 public:
  Spark() {
    auto move = add<MoveBehavior>();
         move->LinearSpeed.set(math::random::get(-2.0f, 2.0f));
         move->AngularSpeed.set(math::random::get(-5.0f, 5.0f));

    auto tex = add<SpriteComponent>();
         tex->Depth = 10.0f;
         tex->Material = MaterialDatabase::instance()->get("bullet");
         tex->Transform = math::make_scale(0.2f);

    auto deleter = add<TimedDeleteBehavior>();
         deleter->Life = 10;
         deleter->on_delete.connect([&](){ --particle_count; });
  }
};



int main(int argc, char** argv) {

  // initialize Swift2D
  Application app(argc, argv);

  // load resources ------------------------------------------------------------
  MaterialDatabase::instance()->add("background", ShadelessTextureMaterial::create_from_file(app.get_resource("images", "bg.jpg")));
  MaterialDatabase::instance()->add("ship",       ShadelessTextureMaterial::create_from_file(app.get_resource("images", "ship.png")));
  MaterialDatabase::instance()->add("bullet",     ShadelessTextureMaterial::create_from_file(app.get_resource("images", "bullet.png")));

  MaterialDatabase::instance()->add("planet1",    BumpTextureMaterial::create_from_files(app.get_resource("images", "planet_diffuse2.png"), app.get_resource("images", "planet_normal2.png")));
  MaterialDatabase::instance()->add("planet2",    BumpTextureMaterial::create_from_files(app.get_resource("images", "planet_diffuse.png"), app.get_resource("images", "planet_normal.png")));

  MaterialDatabase::instance()->add("light",      PointLightMaterial::create_from_file(app.get_resource("images", "light.png")));
  MaterialDatabase::instance()->add("sun",        DirectionalLightMaterial::create(math::vec3(1, 1, -1)));


  // window setup --------------------------------------------------------------
  auto window = Window::create();
  // window->Fullscreen = true;

  // example scene setup -------------------------------------------------------
  auto scene = SceneObject::create();

  auto music = scene->add<SoundComponent>();
       music->Sound = Sound::create_from_file(app.get_resource("audio", "music.ogg"));
       music->Volume = 0.1f;
       music->play();

  auto field = scene->add<CircularShape>();
       field->Transform = math::make_scale(4);

  auto sun = scene->add<LightComponent>();
       sun->Transform = math::make_scale(15) * math::make_translate(-0.2, 0.2);
       sun->Material = MaterialDatabase::instance()->get("sun");

  auto bg = scene->add<SpriteComponent>();
       bg->Depth = -1000.0f;
       bg->Material = MaterialDatabase::instance()->get("background");
       bg->Transform = math::make_scale(2.f);

  auto camera = scene->add<CameraComponent>();
       camera->Size = math::vec2(2.f, 2.f);

  auto fps = scene->add<TextComponent>();
       fps->Transform = math::make_translate(-1.9, -1.9);
       fps->Text = Text::create("FPS", "sans", 12);
       fps->Depth = 1000.f;
       fps->InScreenSpace = true;

  // planet
  auto planet1 = scene->add_object();
       planet1->Transform = math::make_translate(-0.9, -0.5);
  auto sprite2 = planet1->add<SpriteComponent>();
       sprite2->Depth = 0.0f;
       sprite2->Material = MaterialDatabase::instance()->get("planet1");

  auto planet2 = scene->add_object();
       planet2->Transform = math::make_translate(-1.2, 1.0);
  auto sprite3 = planet2->add<SpriteComponent>();
       sprite3->Depth = 0.0f;
       sprite3->Material = MaterialDatabase::instance()->get("planet2");

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

  auto light = player->add<LightComponent>();
       light->Depth = 1.0f;
       light->Transform = math::make_scale(20);
       light->Material = MaterialDatabase::instance()->get("light");

  // rendering pipeline --------------------------------------------------------
  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);

  Renderer renderer;
  renderer.set_pipeline(pipeline);

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();

  Ticker ticker(1.0 / 60.0);
  ticker.on_tick.connect([&]() {

    std::stringstream sstr;
    sstr.precision(1);
    sstr.setf(std::ios::fixed, std::ios::floatfield);
    sstr << "FPS: " << pipeline->rendering_fps() << " / "
         << pipeline->application_fps() << " Particles: " << particle_count;
    fps->Text->Content = sstr.str();

    window->process_input();
    scene->update(timer.get_elapsed());
    renderer.process(scene, camera);
    timer.reset();
  });

  window->on_close.connect([&](){
    renderer.stop();
    app.stop();
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
      app.stop();
    } else if (key == swift::Key::SPACE && action != 1) {
      auto bullet = std::make_shared<Bullet>(scene);
      scene->add_object(bullet);
      bullet->Transform = player->Transform();

      ++particle_count;

      for (int i(0); i<5; ++i) {
        auto spark = scene->add_object(std::make_shared<Spark>());
        spark->Transform = player->Transform();
        ++particle_count;
      }
    }
  });

  app.start();

  return 0;
}
