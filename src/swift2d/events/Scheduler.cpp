////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/events/Scheduler.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Scheduler::Scheduler()
  : current_id_(0) {}

////////////////////////////////////////////////////////////////////////////////

Scheduler::~Scheduler() {
  for (auto& c: tasks_) {
    delete c.second.first;
  }
}

////////////////////////////////////////////////////////////////////////////////

int Scheduler::execute_delayed(double delay, std::function<void()> callback) {

  boost::asio::deadline_timer* timer = new boost::asio::deadline_timer(MainLoop::get().get_io_service(), boost::posix_time::microseconds(1000000.0 * delay));

  tasks_.insert(std::make_pair(current_id_, std::make_pair(timer, callback)));
  timer->async_wait(boost::bind(&Scheduler::self_callback, this, timer, current_id_));

  return current_id_++;
}

////////////////////////////////////////////////////////////////////////////////

void Scheduler::cancel(int id) {
  auto callback(tasks_.find(id));

  if (callback != tasks_.end()) {
    delete callback->second.first;
    tasks_.erase(id);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Scheduler::self_callback(boost::asio::deadline_timer* timer, int id) {
  auto callback(tasks_.find(id));

  if (callback != tasks_.end()) {
    callback->second.second();
    delete callback->second.first;
    tasks_.erase(id);
  }
}

////////////////////////////////////////////////////////////////////////////////


}
