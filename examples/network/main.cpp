////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/swift2d.hpp>

#include "Network.hpp"

// main ------------------------------------------------------------------------
int main(int argc, char** argv) {

  // initialize Swift2D
  swift::Application app(argc, argv);
  swift::Network::instance()->connect("myTestGame");

  swift::Ticker ticker(1.0 / 60.0);
  ticker.on_tick.connect([&]() {
    swift::Network::instance()->update();
  });

  app.start();

  swift::Network::instance()->disconnect();

  return 0;
}

