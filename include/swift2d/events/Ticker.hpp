////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_TICKER_HPP
#define SWIFT2D_EVENTS_TICKER_HPP

// includes  ------------------------------------------------------------------
#include <ev++.h>
#include <swift2d/events/Scheduler.hpp>
#include <swift2d/events/Signal.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class Ticker {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------- signals
  Signal<> on_tick;

  // ---------------------------------------------------- construction interface
  Ticker(double tick_time);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void self_callback(ev::timer& timer, int revents);
  ev::timer timer_;
};

// -----------------------------------------------------------------------------

}

#endif /* SWIFT2D_EVENTS_TICKER_HPP */
