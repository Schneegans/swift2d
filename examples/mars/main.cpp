////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "Mover.hpp"
#include "Bullet.hpp"

#include <iostream>

using namespace swift;

int main(int argc, char** argv) {

  // initialize Swift2D
  Application::instance()->init(argc, argv);

  Object::init<Mover>();
  Object::init<Bullet>();

  // load resources ------------------------------------------------------------
  TextureDatabase::instance()->add("point_light", Texture::create(Application::instance()->get_resource("images", "light.png")));
  TextureDatabase::instance()->add("bullet", Texture::create(Application::instance()->get_resource("images", "bullet.png")));

  auto mat1 = SpriteMaterial::create();
  mat1->DiffuseTexture = TextureDatabase::instance()->get("bullet");
  mat1->Emit = 1.0;
  mat1->Glow = 1.0;
  MaterialDatabase::instance()->add("bullet", mat1);

  auto mat2 = PointLightMaterial::create_from_database("point_light");
  mat2->Color = Color(0.4, 0.3, 1.0);
  MaterialDatabase::instance()->add("light", mat2);

  MaterialDatabase::instance()->add("sun1",  DirectionalLightMaterial::create(math::vec3(0.1, 1, 0), Color(1, 0.5, 1.0)));
  MaterialDatabase::instance()->add("sun2",  DirectionalLightMaterial::create(math::vec3(-1, -1, 0), Color(0.4, 0.8, 1.0)));


  // window setup --------------------------------------------------------------
  auto window = WindowManager::instance()->get_default();
  window->Fullscreen = true;
  window->VSync = true;
  window->HideCursor = true;

  // rendering pipeline --------------------------------------------------------
  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);

  Renderer renderer;
  renderer.set_pipeline(pipeline);

  // example scene setup -------------------------------------------------------
  auto scene = SceneManager::instance()->get_default();

  // auto music = scene->add<SoundComponent>();
  //      music->Sound = Sound::create_from_file(Application::instance()->get_resource("audio", "music.ogg"));
  //      music->Volume = 0.5f;
  //      music->play();

  auto field = scene->add<CircularShape>();
       field->Transform = math::make_scale(4);

  auto camera = scene->add<CameraComponent>();
       camera->Size = math::vec2(2.f, 2.f);
       camera->Parallax = 1.05;

  // auto options = scene->add<GuiComponent>();
  //      options->Resource = Application::instance()->get_resource("gui", "window.html");
  //      options->Size = math::vec2i(1000, 1000);
  //      options->Anchor = math::vec2i(0, 1);
  //      options->on_loaded.connect([&](){
  //        options->add_javascript_callback("start");
  //        options->add_javascript_callback("quit");
  //        options->add_javascript_callback("pause");
  //      });
  //      options->on_javascript_callback.connect([&](std::string const& method) {
  //        if (method == "quit") {
  //          renderer.stop();
  //          Application::instance()->stop();
  //        } else if (method == "pause") {
  //           // music->pause();
  //        } else {
  //         std::cout << "Start!!!" << std::endl;
  //        }
  //      });

  auto menu = scene->add<GuiComponent>();
       menu->Resource = Application::instance()->get_resource("gui", "main_menu.html");
       menu->Size = math::vec2i(525, 180);
       menu->Anchor = math::vec2i(-1, 1);

  auto fps = scene->add<GuiComponent>();
       fps->Resource = Application::instance()->get_resource("gui", "fps.html");
       fps->Size = math::vec2i(240, 35);
       fps->Anchor = math::vec2i(-1, -1);

  auto mouse = scene->add<GuiComponent>();
       mouse->Resource = Application::instance()->get_resource("gui", "mouse.html");
       mouse->Size = math::vec2i(50, 50);
       mouse->Anchor = math::vec2i(-1, -1);

  // scene
  scene->add_object(SceneObject::create_from_file(
    Application::instance()->get_resource("scene", "scene.json")
  ));

  // player
  auto player = scene->add_object(SceneObject::create_from_file(
    Application::instance()->get_resource("scene", "player.json")
  ));
  player->translate(-5, 25);
  player->rotate(1);
  auto body = player->add<DynamicBodyComponent>();
       body->Radius = 1.0f;
       body->Friction = 0.2f;
       body->AngularDamping = 15.f;
       body->on_collision.connect([&](StaticBodyComponent* other){
         std::cout << "col" << std::endl;
       });

  player->get_component<Mover>()->set_camera(camera);

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();

  auto ticker(Ticker::create(1.0 / 60.0));
  ticker->on_tick.connect([&]() {
    renderer.process(scene, camera);

    double time(timer.get_elapsed());
    timer.reset();

    Physics::instance()->update(time);

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
    Application::instance()->stop();
  });

  window->on_resize.connect([&](math::vec2i const& size){
    float height = 2;
    float aspect = 1.0f * size.x() / size.y();
    math::vec2 camera_size(height*aspect, height);
    camera->Size = camera_size;
  });

  window->on_mouse_move.connect([&](math::vec2 const& pos){
    mouse->Offset = pos + math::vec2(-5.f, -45.f);
  });

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (key == swift::Key::ESCAPE) {
      renderer.stop();
      Application::instance()->stop();
    } else if (key == swift::Key::F5 && action != 1) {
      // options->reload();
      // fps->reload();
    } else if (key == swift::Key::SPACE && action != 1) {
      auto bullet = std::make_shared<Bullet>(player->WorldTransform());
      scene->add_object(bullet);
      bullet->shoot(player);
    }
  });

  Interface::instance()->on_cursor_change.connect([&](Cursor pointer){
    mouse->call_javascript("set_active", std::to_string(pointer == Cursor::HAND));
  });

  Application::instance()->start();

  return 0;
}
