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

// shared pointer type definition ----------------------------------------------
class Ticker;
typedef std::shared_ptr<Ticker>       TickerPtr;
typedef std::shared_ptr<const Ticker> ConstTickerPtr;

// -----------------------------------------------------------------------------
class Ticker : public std::enable_shared_from_this<Ticker> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------- signals
  Signal<> on_tick;

  // ---------------------------------------------------- construction interface
  template <typename... Args>
  static TickerPtr create(Args&& ... a) {
    return std::make_shared<Ticker>(a...);
  }

  Ticker(double tick_time);
  ~Ticker();

  void set_tick_time(double tick_time);
  double get_tick_time() const;

  void start();
  void stop();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void self_callback(boost::system::error_code const& error);
  void async_wait();

  boost::asio::deadline_timer timer_;
  double tick_time_;
  bool active_;
};

// -----------------------------------------------------------------------------

}

#endif /* SWIFT2D_EVENTS_TICKER_HPP */
