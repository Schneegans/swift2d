////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/events/MainLoop.hpp>
#include <ev++.h>
#include <iostream>

namespace swift {

MainLoop::MainLoop() {}

MainLoop::~MainLoop() {}

void MainLoop::start() {
  struct ev_loop* loop = EV_DEFAULT;
  ev_run(loop, 0);
}

void MainLoop::stop() {
  struct ev_loop* loop = EV_DEFAULT;
  ev_break(loop, EVBREAK_ONE);
}

}
