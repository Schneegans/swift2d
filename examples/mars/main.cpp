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
  Mover() {
    auto w = WindowManager::instance()->get_default();
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

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Mover"; }

  virtual void accept(SavableObjectVisitor& visitor) {
    MoveBehavior::accept(visitor);
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
  }
};

int main(int argc, char** argv) {

  // initialize Swift2D
  Application app(argc, argv);

  Object::init<Mover>();

  // load resources ------------------------------------------------------------
  TextureDatabase::instance()->add("point_light", Texture::create(app.get_resource("images", "light.png")));

  MaterialDatabase::instance()->add("bullet",     ShadelessTextureMaterial::create_from_file(app.get_resource("images", "bullet.png")));

  auto mat = PointLightMaterial::create_from_database("point_light");
  mat->Color = Color(0.4, 0.3, 1.0);
  MaterialDatabase::instance()->add("light",      mat);

  MaterialDatabase::instance()->add("sun1",       DirectionalLightMaterial::create(math::vec3(0.1, 1, 0), Color(1, 0.5, 1.0)));
  MaterialDatabase::instance()->add("sun2",       DirectionalLightMaterial::create(math::vec3(-1, -1, 0), Color(0.4, 0.8, 1.0)));


  // window setup --------------------------------------------------------------
  auto window = WindowManager::instance()->get_default();
  window->Fullscreen = true;
  window->VSync = false;

  // rendering pipeline --------------------------------------------------------
  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);

  Renderer renderer;
  renderer.set_pipeline(pipeline);

  // example scene setup -------------------------------------------------------
  auto scene = SceneManager::instance()->get_default();

  auto music = scene->add<SoundComponent>();
       music->Sound = Sound::create_from_file(app.get_resource("audio", "music.ogg"));
       music->Volume = 0.1f;
       music->play();

  auto field = scene->add<CircularShape>();
       field->Transform = math::make_scale(4);

  auto camera = scene->add<CameraComponent>();
       camera->Size = math::vec2(2.f, 2.f);


  auto menu = scene->add<GuiComponent>();
       menu->Resource = app.get_resource("gui", "window.html");
       menu->Size = math::vec2i(1000, 1000);
       menu->Anchor = math::vec2i(0, 1);
       menu->on_loaded.connect([&](){
         menu->add_javascript_callback("start");
         menu->add_javascript_callback("quit");
         menu->add_javascript_callback("pause");
       });
       menu->on_javascript_callback.connect([&](std::string const& method) {
         if (method == "quit") {
           renderer.stop();
           app.stop();
         } else if (method == "pause") {
            music->pause();
         } else {
          std::cout << "Start!!!" << std::endl;
         }
       });

  auto fps = scene->add<GuiComponent>();
       fps->Resource = app.get_resource("gui", "fps.html");
       fps->Size = math::vec2i(240, 35);
       fps->Anchor = math::vec2i(-1, -1);

  auto video = scene->add<GuiComponent>();
       video->Resource = app.get_resource("gui", "video.html");
       video->Size = math::vec2i(420, 315);
       video->Anchor = math::vec2i(1, -1);

  // scene
  scene->add_object(SceneObject::create_from_file(
    app.get_resource("scene", "scene.json")
  ));

  // player
  auto player = scene->add_object(SceneObject::create_from_file(
    app.get_resource("scene", "player.json")
  ));

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();

  auto ticker(Ticker::create(1.0 / 60.0));
  ticker->on_tick.connect([&]() {
    renderer.process(scene, camera);

    double time(timer.get_elapsed());
    timer.reset();

    std::stringstream sstr;
    sstr.precision(1);
    sstr.setf(std::ios::fixed, std::ios::floatfield);
    sstr << "FPS: " << pipeline->rendering_fps() << " / "
         << pipeline->application_fps();

    fps->call_javascript("set_fps_text", sstr.str());

    window->process_input();
    scene->update(time);
    Interface::instance()->update();
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
    } else if (key == swift::Key::F5 && action != 1) {
      menu->reload();
      fps->reload();
    }
  });

  app.start();

  return 0;
}
