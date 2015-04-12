////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/events/Timer.hpp>

// external headers
#include <iostream>
#include <chrono>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Timer::Timer()
  : start_(-1.f) {}

////////////////////////////////////////////////////////////////////////////////

void Timer::start() {
  start_ = get_now();
}

////////////////////////////////////////////////////////////////////////////////

double Timer::reset() {
  double now(get_now());
  double elapsed(now - start_);
  start_ = now;
  return elapsed;
}

////////////////////////////////////////////////////////////////////////////////

double Timer::get_elapsed() const {
  return get_now() - start_;
}

////////////////////////////////////////////////////////////////////////////////

bool Timer::is_running() const {
  return start_ != -1.f;
}

////////////////////////////////////////////////////////////////////////////////

double Timer::get_now() {
  auto time = std::chrono::system_clock::now();
  auto since_epoch = time.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count() * 0.000001;
}

////////////////////////////////////////////////////////////////////////////////

}
