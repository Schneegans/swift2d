////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_TICKER_HPP
#define SWIFT2D_EVENTS_TICKER_HPP

#include <ev++.h>
#include <swift2d/events/Scheduler.hpp>
#include <swift2d/events/Signal.hpp>

namespace swift {

class Ticker {
 public:
  Ticker(double tick_time);
  ~Ticker() {}
  ;

  Signal<> on_tick;

 private:
  void self_callback(ev::timer& timer, int revents);
  ev::timer timer_;
};

}

#endif /* SWIFT2D_EVENTS_TICKER_HPP */
