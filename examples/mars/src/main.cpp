////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "../include/Mover.hpp"
#include "../include/Bullet.hpp"
#include "../include/GuiManager.hpp"
#include "../include/AssetManager.hpp"
#include "../include/MusicManager.hpp"

#include <iostream>

using namespace swift;

int main(int argc, char** argv) {

  // initialize Swift2D
  Application::instance()->init(argc, argv);

  Object::init<Mover>();
  Object::init<Bullet>();

  // load resources ------------------------------------------------------------
  AssetManager assets;

  // window setup --------------------------------------------------------------
  auto window = WindowManager::instance()->get_default();
  window->Fullscreen = false;
  window->VSync = true;
  window->HideCursor = true;

  // rendering pipeline --------------------------------------------------------
  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);

  Renderer renderer(pipeline);

  // example scene setup -------------------------------------------------------
  auto scene = SceneManager::instance()->get_default();

  auto camera = scene->add<CameraComponent>();
       camera->Size = math::vec2(2.f, 2.f);
       camera->Parallax = 1.05;

  // scene
  auto world = scene->add_object(SceneObject::create_from_file(
    Application::instance()->get_resource("scene", "scene.json")
  ));

  auto emitters(world->get_components<ParticleEmitterComponent>());
  for (auto emitter: emitters) {
    world->get_component<ParticleSystemComponent>()->add_emitter(emitter);
  }

  // player
  auto player = scene->add_object(SceneObject::create_from_file(
    Application::instance()->get_resource("scene", "player.json")
  ));
  player->translate(2, 5);
  player->get_component<Mover>()->set_camera(camera);
  player->get_component<SpriteParticleSystemComponent>()->add_emitter(
    player->get_component<ParticleEmitterComponent>()
  );

  // gui
  GuiManager::instance()->on_quit.connect([&](){
    renderer.stop();
    Application::instance()->stop();
  });

  // play music ----------------------------------------------------------------
  MusicManager::instance()->play_next();

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();

  renderer.on_frame.connect([&]() {

    renderer.process(scene, camera);

    double time(timer.reset());
    Physics::instance()->update(time);

    window->process_input();
    scene->update(time);

    GuiManager::instance()->update(renderer.RenderFPS.fps(), renderer.AppFPS.fps());
  });

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

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (key == swift::Key::SPACE && action != 1) {
      auto bullet = std::make_shared<Bullet>(player->WorldTransform());
      scene->add_object(bullet);
      bullet->shoot(player);
    }
  });

  Application::instance()->start();

  return 0;
}
