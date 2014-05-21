////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_SCHEDULER_HPP
#define SWIFT2D_EVENTS_SCHEDULER_HPP

// includes  ------------------------------------------------------------------
#include <ev++.h>
#include <functional>
#include <map>

namespace swift {

// -----------------------------------------------------------------------------
class Scheduler {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  void execute_delayed(std::function<void()> callback, double delay);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void self_callback(ev::timer& timer, int revents);

  std::map<ev::timer*, std::function<void()>> tasks_;
};

// -----------------------------------------------------------------------------

}

#endif /* SWIFT2D_EVENTS_SCHEDULER_HPP */
