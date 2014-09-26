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

  // ------------------------------------------------------------ public methods
  void execute_delayed(double delay, std::function<void()> callback);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
   void self_callback(boost::asio::deadline_timer* timer, int revents);

   std::map<boost::asio::deadline_timer*, std::function<void()> > tasks_;
};

// -----------------------------------------------------------------------------

}

#endif /* SWIFT2D_EVENTS_SCHEDULER_HPP */
