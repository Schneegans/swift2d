////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/swift2d.hpp>

#include "Network.hpp"
#include "NetworkObject.hpp"


// main ------------------------------------------------------------------------
int main(int argc, char** argv) {

  // initialize Swift2D
  swift::Application app(argc, argv);
  swift::Network::instance()->connect("myTestGame");

  auto window = swift::Window::create();
  window->Open = true;
  swift::NetworkObject test;

  swift::Ticker ticker(1.0 / 60.0);
  ticker.on_tick.connect([&]() {
    window->process_input();
    swift::Network::instance()->update();
  });

  window->on_close.connect([&](){
    app.stop();
  });

  std::vector<swift::NetworkObject*> my_objects;

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (action != 1) {
      switch(key) {
        case swift::Key::ESCAPE:
          app.stop();
          break;
        case swift::Key::SPACE:
          my_objects.push_back(new swift::NetworkObject());
          swift::Network::instance()->distribute_object(my_objects.back());
          break;
        case swift::Key::A:
          for (auto o: my_objects) { ++o->test_var_1; ++o->test_var_2; }
          break;
        case swift::Key::D:
          for (auto o: my_objects) delete o;
          my_objects.clear();
          break;
      }
    }
  });

  app.start();

  swift::Network::instance()->disconnect();

  return 0;
}

