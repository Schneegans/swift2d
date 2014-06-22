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
        if (key == Key::W) {
          LinearSpeed.set(0);
          get_user()->get_components<ParticleSystemComponent>()[0]->Emitter()->Density = 0.0;
          get_user()->get_components<ParticleSystemComponent>()[1]->Emitter()->Density = 0.0;
          get_user()->get_components<ParticleSystemComponent>()[2]->Emitter()->Density = 0.0;
        }
        if (key == Key::S) LinearSpeed.set(0);
        if (key == Key::A) AngularSpeed.set(0);
        if (key == Key::D) AngularSpeed.set(0);
      } else if (action == 1) {
        if (key == Key::W) {
          LinearSpeed.set( 10);
          get_user()->get_components<ParticleSystemComponent>()[0]->Emitter()->Density = 100.0;
          get_user()->get_components<ParticleSystemComponent>()[1]->Emitter()->Density = 100.0;
          get_user()->get_components<ParticleSystemComponent>()[2]->Emitter()->Density = 15.0;
        }
        if (key == Key::S) LinearSpeed.set(-10);
        if (key == Key::A) AngularSpeed.set(-2 );
        if (key == Key::D) AngularSpeed.set( 2 );
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
  TextureDatabase::instance()->add("smoke", Texture::create(app.get_resource("images", "smoke.png")));
  TextureDatabase::instance()->add("fire", Texture::create(app.get_resource("images", "fire.png")));
  TextureDatabase::instance()->add("point_light", Texture::create(app.get_resource("images", "light.png")));

  MaterialDatabase::instance()->add("background", ShadelessTextureMaterial::create_from_file(app.get_resource("images", "bg.jpg")));
  MaterialDatabase::instance()->add("ship",       ShadelessTextureMaterial::create_from_file(app.get_resource("images", "ship.png")));
  MaterialDatabase::instance()->add("bullet",     ShadelessTextureMaterial::create_from_file(app.get_resource("images", "bullet.png")));

  MaterialDatabase::instance()->add("planet2",    ShadelessTextureMaterial::create_from_file(app.get_resource("images", "planet_diffuse2.png")));
  MaterialDatabase::instance()->add("planet1",    BumpTextureMaterial::create_from_files(app.get_resource("images", "planet_diffuse.png"),
                                                                                         app.get_resource("images", "planet_normal.png"),
                                                                                         0.1, 50.f, 0.8f));
  MaterialDatabase::instance()->add("planet3",    BumpTextureMaterial::create_from_files(app.get_resource("images", "planet_diffuse3.png"),
                                                                                         app.get_resource("images", "planet_normal3.png"),
                                                                                         0.1, 20.f, 0.3f));

  auto mat = PointLightMaterial::create_from_database("point_light");
  mat->Color = Color(0.4, 0.3, 1.0);
  MaterialDatabase::instance()->add("light",      mat);

  MaterialDatabase::instance()->add("sun1",       DirectionalLightMaterial::create(math::vec3(0.1, 1, 0), Color(1, 0.5, 1.0)));
  MaterialDatabase::instance()->add("sun2",       DirectionalLightMaterial::create(math::vec3(-1, -1, 0), Color(0.4, 0.8, 1.0)));


  // window setup --------------------------------------------------------------
  auto window = WindowManager::instance()->get_default();
  // window->Fullscreen = true;

  // example scene setup -------------------------------------------------------
  auto scene = SceneManager::instance()->get_default();

  // auto music = scene->add<SoundComponent>();
  //      music->Sound = Sound::create_from_file(app.get_resource("audio", "music.ogg"));
  //      music->Volume = 0.1f;
  //      music->play();

  auto field = scene->add<CircularShape>();
       field->Transform = math::make_scale(4);

  auto sun1 = scene->add<LightComponent>();
       sun1->Material = MaterialDatabase::instance()->get("sun1");

  auto sun2 = scene->add<LightComponent>();
       sun2->Material = MaterialDatabase::instance()->get("sun2");

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
       planet1->Transform = math::make_translate(-0.9, 0.5) * math::make_scale(1.2f);
  auto sprite1 = planet1->add<SpriteComponent>();
       sprite1->Depth = 0.0f;
       sprite1->Material = MaterialDatabase::instance()->get("planet1");
  auto rot = planet1->add<MoveBehavior>();
       rot->AngularSpeed = 0.1f;

  auto planet2 = scene->add_object();
       planet2->Transform = math::make_translate(-1.2, -2.0) * math::make_scale(2.2f);
  auto sprite2 = planet2->add<SpriteComponent>();
       sprite2->Depth = 40.0f;
       sprite2->Material = MaterialDatabase::instance()->get("planet2");
       rot = planet2->add<MoveBehavior>();
       rot->AngularSpeed = -0.1f;

  auto planet3 = scene->add_object();
       planet3->Transform = math::make_translate(1.2, -1.0) * math::make_scale(1.5f);;
  auto sprite3 = planet3->add<SpriteComponent>();
       sprite3->Depth = 0.0f;
       sprite3->Material = MaterialDatabase::instance()->get("planet3");
       rot = planet3->add<MoveBehavior>();
       rot->AngularSpeed = 0.03f;

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

  // exhaust
  auto smoke_particles = TextureParticleEmitter::create();
       smoke_particles->Life = 5.0f;
       smoke_particles->LifeVariance = 3.0f;
       smoke_particles->StartScale = 0.1f;
       smoke_particles->EndScale = 5.0f;
       smoke_particles->StartColor = Color(0.0, 0.0, 0.0);
       smoke_particles->EndColor = Color(1, 1, 1);
       smoke_particles->RotationSpeedVariance = 2.f;
       smoke_particles->Direction = math::vec2(-2.f, 0.f);
       smoke_particles->Texture = TextureDatabase::instance()->get("smoke");

  auto smoke = player->add<ParticleSystemComponent>();
       smoke->Depth = 0.5f;
       smoke->Transform = math::make_scale(2) * math::make_translate(-0.5, 0);
       smoke->Emitter = smoke_particles;

  auto fire_particles = TextureParticleEmitter::create();
       fire_particles->Life = 1.0f;
       fire_particles->LifeVariance = 0.5f;
       fire_particles->StartScale = 0.2f;
       fire_particles->EndScale = 2.0f;
       fire_particles->StartOpacity = 0.5f;
       fire_particles->BlendAdditive = true;
       fire_particles->StartColor = Color(1, 1, 1);
       fire_particles->EndColor = Color(0.8, 0.0, 0.0);
       fire_particles->Direction = math::vec2(-2.f, 0.f);
       fire_particles->Texture = TextureDatabase::instance()->get("fire");

  auto fire = player->add<ParticleSystemComponent>();
       fire->Depth = 0.6f;
       fire->Transform = math::make_scale(2) * math::make_translate(-0.5, 0);
       fire->Emitter = fire_particles;

  auto fire_light_particles = LightParticleEmitter::create();
       fire_light_particles->Life = 1.0f;
       fire_light_particles->LifeVariance = 0.5f;
       fire_light_particles->StartScale = 10.f;
       fire_light_particles->EndScale = 20.0f;
       fire_light_particles->StartOpacity = 0.5f;
       fire_light_particles->StartColor = Color(1, 0.1, 0.1);
       fire_light_particles->EndColor = Color(1.0, 0.0, 0.0);
       fire_light_particles->Direction = math::vec2(-2.f, 0.f);
       fire_light_particles->Texture = TextureDatabase::instance()->get("point_light");

  auto fire_light = player->add<ParticleSystemComponent>();
       fire_light->Depth = 0.6f;
       fire_light->Transform = math::make_scale(2) * math::make_translate(-0.5, 0);
       fire_light->Emitter = fire_light_particles;

  // rendering pipeline --------------------------------------------------------
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

    std::stringstream sstr;
    sstr.precision(1);
    sstr.setf(std::ios::fixed, std::ios::floatfield);
    sstr << "FPS: " << pipeline->rendering_fps() << " / "
         << pipeline->application_fps() << " Particles: "
         << particle_count + smoke->get_particle_count()
                           + fire->get_particle_count()
                           + fire_light->get_particle_count();
    fps->Text->Content = sstr.str();

    window->process_input();
    scene->update(time);
    renderer.process(scene, camera);
  });

  ticker->start();

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

      for (int i(0); i<25; ++i) {
        auto spark = scene->add_object(std::make_shared<Spark>());
        spark->Transform = player->Transform();
        ++particle_count;
      }
    }
  });

  app.start();

  return 0;
}
