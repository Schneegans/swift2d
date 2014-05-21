////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/events/Timer.hpp>

// external headers
#include <iostream>
#include <chrono>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void Timer::start() {
  start_ = get_now();
}

////////////////////////////////////////////////////////////////////////////////

void Timer::reset() {
  start_ = get_now();
}

////////////////////////////////////////////////////////////////////////////////

double Timer::get_elapsed() const {
  return get_now() - start_;
}

////////////////////////////////////////////////////////////////////////////////

double Timer::get_now() {

  auto time = std::chrono::system_clock::now();
  auto since_epoch = time.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count() * 0.000001;
}

////////////////////////////////////////////////////////////////////////////////

}
