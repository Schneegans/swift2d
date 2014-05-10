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
using namespace swift::cores;

int main(int argc, char** argv) {

  // initialize FibreGlass3D
  init(argc, argv);

  // example scene setup -------------------------------------------------------
  auto scene = Scene::create();
  scene->name.set("main_scene");

  // create a light source
  auto light = scene->add_node(Node::create());
  light->add_core<Transformation>()->config()
    .set_transform(math::make_scale(0.5f));
  light->add_core<Sprite>()->config()
    .set_texture("data/test.png");

  // create a view point
  auto eye = Node::create();
  eye->add_core<Transformation>()->config()
    .set_transform(math::make_translate(0.f, 0.f, 1.5f));
  eye->add_core<View>();

  // create a screen
  auto screen = scene->add_node(Node::create());
  screen->add_core<Transformation>()->config()
    .set_transform(math::make_translate(0.f, 0.f, 1.f));
  screen->add_core<Screen>()->config()
    .set_size(glm::vec2(1.6f, 0.9f));
  screen->add_core<Group>()->add_node(eye);

  // rendering pipeline --------------------------------------------------------
  auto window = Window::create();
  auto pipeline = Pipeline::create();
  pipeline->set_output_window(window);
  pipeline->camera
    .set_eye_left(eye->get_paths()[0])
    .set_screen_left(screen->get_paths()[0])
    .set_scene("main_scene");

  Renderer renderer({pipeline});

  // main loop -----------------------------------------------------------------
  Timer timer;
  timer.start();
  Ticker ticker(1.0 / 60.0);
  ticker.on_tick.connect([&]() {
    renderer.queue_draw({scene.get()});
    window->process_input();
  });

  MainLoop loop;

  window->on_close.connect([&](){
    renderer.stop();
    loop.stop();
  });

  loop.start();

  return 0;
}
