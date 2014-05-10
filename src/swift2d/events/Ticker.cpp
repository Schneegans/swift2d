////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/events/Ticker.hpp>

namespace swift {

Ticker::Ticker(double tick_time) {

  timer_.set<Ticker, &Ticker::self_callback>(this);
  timer_.start(tick_time, tick_time);
}

void Ticker::self_callback(ev::timer& timer, int revents) { on_tick.emit(); }

}
