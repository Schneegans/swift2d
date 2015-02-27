////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
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

Scheduler::Scheduler(Scheduler const& to_copy)
  : current_id_(0) {}

////////////////////////////////////////////////////////////////////////////////

Scheduler::~Scheduler() {
  std::unique_lock<std::mutex> lock(mutex_);
  for (auto& c: tasks_) {
    delete c.second.first;
  }
}

////////////////////////////////////////////////////////////////////////////////

int Scheduler::execute_delayed(double delay, std::function<void()> callback) {

  boost::asio::deadline_timer* timer = new boost::asio::deadline_timer(MainLoop::get().get_io_service(), boost::posix_time::microseconds(1000000.0 * delay));

  int id;

  {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.insert(std::make_pair(current_id_, std::make_pair(timer, callback)));
    id = current_id_++;
  }

  timer->async_wait(boost::bind(&Scheduler::self_callback, this, timer, current_id_-1));

  return id;
}

////////////////////////////////////////////////////////////////////////////////

void Scheduler::cancel(int id) {
  std::unique_lock<std::mutex> lock(mutex_);
  auto callback(tasks_.find(id));

  if (callback != tasks_.end()) {
    delete callback->second.first;
    tasks_.erase(id);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Scheduler::self_callback(boost::asio::deadline_timer* timer, int id) {
  std::function<void()> callback;

  {
    std::unique_lock<std::mutex> lock(mutex_);
    auto callback_it(tasks_.find(id));

    if (callback_it != tasks_.end()) {
      callback = callback_it->second.second;
      delete callback_it->second.first;
      tasks_.erase(id);
    }
  }

  if (callback) {
    callback();
  }
}

////////////////////////////////////////////////////////////////////////////////


}
