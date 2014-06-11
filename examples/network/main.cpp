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

  window->on_key_press.connect([&](swift::Key key, int scancode, int action, int mods) {
    if (key == swift::Key::ESCAPE) {
      app.stop();
    } else if (key == swift::Key::SPACE && action != 1) {
      swift::Logger::LOG_DEBUG << "Creating object." << std::endl;
      auto some_object(new swift::NetworkObject());
      swift::Network::instance()->distribute_object(some_object);
    }
  });

  app.start();

  swift::Network::instance()->disconnect();

  return 0;
}

