////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/events/Ticker.hpp>

#include <boost/bind.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Ticker::Ticker(double tick_time)
  : timer_(MainLoop::get().get_io_service(), boost::posix_time::microseconds(1000000.0*tick_time))
  , tick_time_(tick_time)
  , active_(false) {}

////////////////////////////////////////////////////////////////////////////////

Ticker::~Ticker() {
  on_tick.disconnect_all();
  stop();
}

////////////////////////////////////////////////////////////////////////////////

void Ticker::set_tick_time(double tick_time) {
  tick_time_ = tick_time;
}

////////////////////////////////////////////////////////////////////////////////

double Ticker::get_tick_time() const {
  return tick_time_;
}

////////////////////////////////////////////////////////////////////////////////

void Ticker::start() {
  active_ = true;
  async_wait();
}

////////////////////////////////////////////////////////////////////////////////

void Ticker::stop() {
  active_ = false;
  timer_.cancel();
}

////////////////////////////////////////////////////////////////////////////////

void Ticker::self_callback(boost::system::error_code const& error) {
  if (error !=  boost::asio::error::operation_aborted && active_) {
    async_wait();
    on_tick.emit();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Ticker::async_wait() {
  timer_.expires_from_now(boost::posix_time::microseconds(1000000.0*tick_time_));
  timer_.async_wait(boost::bind(&Ticker::self_callback, shared_from_this(), _1));
}

////////////////////////////////////////////////////////////////////////////////

}

