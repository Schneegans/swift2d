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
         tex->Emit = 1.0f;
         tex->Diffuse = TextureDatabase::instance()->get("bullet");
         tex->Transform = math::make_scale(1.5f);

    auto light = add<PointLightComponent>();
         light->Depth = 1.0f;
         light->Transform = math::make_scale(5.0f);
         light->Tex = TextureDatabase::instance()->get("light");

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
  TextureDatabase::instance()->add("light", Texture::create("light.png"));
  TextureDatabase::instance()->add("bullet", Texture::create("bullet.png"));
  TextureDatabase::instance()->add("background", Texture::create("bg.png"));
  TextureDatabase::instance()->add("diffuse", Texture::create("diffuse.png"));
  TextureDatabase::instance()->add("normal", Texture::create("normal.png"));
  TextureDatabase::instance()->add("ship", Texture::create("ship.png"));


  // window setup --------------------------------------------------------------
  auto window = Window::create();
  // window->Fullscreen = true;

  // example scene setup -------------------------------------------------------
  auto scene = SceneObject::create();

  auto field = scene->add<CircularShape>();
       field->Transform = math::make_scale(2);

  auto sun = scene->add<PointLightComponent>();
       sun->Transform = math::make_scale(5) * math::make_translate(-0.5, 0.5);
       sun->Tex = TextureDatabase::instance()->get("light");

  auto bg = scene->add<SpriteComponent>();
       bg->Depth = -1000.0f;
       bg->Emit = 1.0f;
       bg->Diffuse = TextureDatabase::instance()->get("background");
       bg->Transform = math::make_scale(2.f);


  auto camera = scene->add<CameraComponent>();
  camera->Size = math::vec2(2.f, 2.f);

  // planet
  auto planet = scene->add_object();

  auto sprite = planet->add<SpriteComponent>();
       sprite->Depth = 0.0f;
       sprite->Diffuse = TextureDatabase::instance()->get("diffuse");
       sprite->Normal = TextureDatabase::instance()->get("normal");

  auto boing = planet->add<SoundComponent>();
       boing->set_sound(new Sound("sound.wav"));

  // player
  auto player = scene->add_object();
       player->Transform = math::make_scale(0.1);

  auto mover = std::make_shared<Mover>(window);
  player->add(mover);

  auto listener = player->add<ListenerComponent>();
       listener->Volume = 10.0;

  auto ship = player->add<SpriteComponent>();
       ship->Depth = 1.0f;
       ship->Diffuse = TextureDatabase::instance()->get("ship");

  auto light_object = scene->add_object();
  auto light = light_object->add<PointLightComponent>();
       light->Depth = 1.0f;
       light->Transform = math::make_scale(1);
       light->Tex = TextureDatabase::instance()->get("light");

  // todo: screen aligned sprites!
  player->Transform.on_change().connect([&](math::mat3 const& mat) {
    light_object->Transform.set(math::make_translate(math::get_position(mat)));
  });

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
