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
#include <swift2d/events/MainLoop.hpp>
#include <map>

#include <boost/asio/deadline_timer.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class SWIFT_DLL Scheduler {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  Scheduler();

  // ------------------------------------------------------------ public methods
  int execute_delayed(double delay, std::function<void()> callback);
  void cancel(int id);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
   void self_callback(boost::asio::deadline_timer* timer, int id);

   std::map<int, std::pair<boost::asio::deadline_timer*, std::function<void()>>> tasks_;
   int current_id_;
};

// -----------------------------------------------------------------------------

}

#endif /* SWIFT2D_EVENTS_SCHEDULER_HPP */
