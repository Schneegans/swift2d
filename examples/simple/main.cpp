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


int main(int argc, char** argv) {

  // initialize Swift2D
  Application app(argc, argv);

  Object::init<Mover>();

  // load resources ------------------------------------------------------------
  TextureDatabase::instance()->add("point_light", Texture::create(app.get_resource("images", "light.png")));
  MaterialDatabase::instance()->add("sun1",       DirectionalLightMaterial::create(math::vec3(0.1, 1, 0), Color(1, 0.5, 1.0)));
  MaterialDatabase::instance()->add("sun2",       DirectionalLightMaterial::create(math::vec3(-1, -1, 0), Color(0.4, 0.8, 1.0)));

  // window setup --------------------------------------------------------------
  auto window = WindowManager::instance()->get_default();

  // rendering pipeline --------------------------------------------------------
  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);

  Renderer renderer;
  renderer.set_pipeline(pipeline);

  // example scene setup -------------------------------------------------------
  auto scene = SceneManager::instance()->get_default();

  auto camera = scene->add<CameraComponent>();
       camera->Size = math::vec2(2.f, 2.f);

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
    }
  });

  app.start();

  return 0;
}
