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
  ~Ticker();

  void set_tick_time(double tick_time);
  double get_tick_time() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void self_callback(int revents);
  void async_wait ();

  boost::asio::deadline_timer* timer_;
  double tick_time_;
};

// -----------------------------------------------------------------------------

}

#endif /* SWIFT2D_EVENTS_TICKER_HPP */
