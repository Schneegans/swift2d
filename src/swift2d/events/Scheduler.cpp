////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/events/Scheduler.hpp>

#include <boost/bind.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void Scheduler::execute_delayed(std::function<void()> callback, double delay) {

  boost::asio::deadline_timer* timer = new boost::asio::deadline_timer(MainLoop::get().get_io_service(), boost::posix_time::microseconds(1000000.0 * delay));

  tasks_.insert(std::make_pair(timer, callback));
  timer->async_wait(boost::bind(&Scheduler::self_callback, this, timer, 0));
}

////////////////////////////////////////////////////////////////////////////////

void Scheduler::self_callback(boost::asio::deadline_timer* timer, int revents) {
  auto callback(tasks_.find(timer));

  if (callback != tasks_.end()) {
    callback->second();
    tasks_.erase(timer);
    delete timer;
  }
}

////////////////////////////////////////////////////////////////////////////////


}
