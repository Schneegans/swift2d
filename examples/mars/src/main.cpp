////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "../include/Mover.hpp"
#include "../include/Bullet.hpp"
#include "../include/Planet.hpp"
#include "../include/Asteroid.hpp"
#include "../include/GuiManager.hpp"
#include "../include/AssetManager.hpp"
#include "../include/MusicManager.hpp"
#include "../include/Explosion.hpp"

#include <iostream>

using namespace swift;

enum State {
  LOADING_LOADING_SCREEN,
  LOADING_SCENE,
  LOADING_MUSIC,
  HIDING_LOADING_SCREEN,
  PLAYING
};

// -----------------------------------------------------------------------------
SceneObjectPtr create_loading_screen() {
  auto scene = SceneManager::instance()->get_default();
  auto loading_screen = scene->add_object(SceneObject::create_from_file(
    Application::instance()->get_resource("scene", "loading_screen.json")
  ));

  return loading_screen;
}

// -----------------------------------------------------------------------------
SceneObjectPtr create_world() {
  auto scene = SceneManager::instance()->get_default();
  auto world = scene->add_object(SceneObject::create_from_file(
    Application::instance()->get_resource("scene", "scene.json")
  ));

  return world;
}

// -----------------------------------------------------------------------------
SceneObjectPtr create_player() {
  auto scene = SceneManager::instance()->get_default();
  auto player = scene->add_object(SceneObject::create_from_file(
    Application::instance()->get_resource("scene", "player.json")
  ));
  player->translate(2, 5);
  player->get_component<Mover>()->set_camera(scene->get_component<CameraComponent>());
  player->get_component<TrailSystemComponent>()->add_emitter(player->get_component<TrailEmitterComponent>().get());

  for (auto emitter: player->get_objects()) {
    emitter->get_component<ParticleSystemComponent>()->add_emitter(
      emitter->get_component<ParticleEmitterComponent>().get()
    );
  }

  return player;
}

int main(int argc, char** argv) {

  // initialize Swift2D
  Application::instance()->init(argc, argv);

  Object::init<Mover>();
  Object::init<Bullet>();
  Object::init<Planet>();
  Object::init<Asteroid>();
  Object::init<Explosion>();

  State state = LOADING_LOADING_SCREEN;
  SceneObjectPtr player;
  SceneObjectPtr world;

  // load resources ------------------------------------------------------------
  AssetManager assets;

  // window setup --------------------------------------------------------------
  auto window = WindowManager::instance()->get_default();
  window->Fullscreen = true;
  window->VSync = true;
  window->HideCursor = true;

  // loading screen ------------------------------------------------------------
  auto loading_screen = create_loading_screen();
  AnimatedFloat loading_screen_opacity(1.0);
  loading_screen_opacity.on_change().connect([loading_screen](float val){
    loading_screen->get_component<SpriteComponent>()->Material()->Diffuse = Color(0, 0, 0, val);
  });
  loading_screen_opacity.on_finish().connect([loading_screen](){
    loading_screen->Enabled = false;
    GuiManager::instance()->show_gui();
  });

  // rendering pipeline --------------------------------------------------------
  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);
  pipeline->LoadingProgress.on_change().connect([&](float val){

    switch (state) {
      case LOADING_LOADING_SCREEN:
        if (val == 1.0) {
          world = create_world();
          player = create_player();
          state = LOADING_SCENE;
        } break;

      case LOADING_SCENE:
        if (val == 1.0) {
          state = LOADING_MUSIC;
        } break;
    }
  });

  Renderer renderer(pipeline);

  // example scene setup -------------------------------------------------------
  auto scene = SceneManager::instance()->get_default();

  auto camera = scene->add<CameraComponent>();
       camera->Size = math::vec2(10.f, 10.f);
       camera->Parallax = 1.05;

  // gui
  GuiManager::instance()->on_quit.connect([&](){
    renderer.stop();
    Application::instance()->stop();
  });


  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();

  scene->update(0);
  Physics::instance()->update(0);
  scene->update(0);

  renderer.on_frame.connect([&]() {
    renderer.process(scene, camera);

    double time(timer.reset());
    if (state == PLAYING) {
      Physics::instance()->update(time);
    }

    loading_screen_opacity.update(time);


    if (state == LOADING_MUSIC) {
      MusicManager::instance()->play_next();
      state = HIDING_LOADING_SCREEN;

    } else if (state == HIDING_LOADING_SCREEN) {
      loading_screen_opacity.set(0.f, 1.f);
      state = PLAYING;
    }


    window->process_input();
    scene->update(time);

    GuiManager::instance()->update(renderer.RenderFPS.fps(), renderer.AppFPS.fps());
  });

  window->on_close.connect([&](){
    renderer.stop();
    Application::instance()->stop();
  });

  window->on_resize.connect([&](math::vec2i const& size){
    float height = 10;
    float aspect = 1.0f * size.x() / size.y();
    math::vec2 camera_size(height*aspect, height);
    camera->Size = camera_size;
  });

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (key == swift::Key::SPACE && action != 1) {
      auto bullet = std::make_shared<Bullet>(player->WorldTransform(), world);
      scene->add_object(bullet);
      bullet->shoot(player);
    }
  });

  Application::instance()->start();

  return 0;
}
