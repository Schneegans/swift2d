////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_FPS_COUNTER_HPP
#define SWIFT2D_FPS_COUNTER_HPP

#include <swift2d/events/Timer.hpp>
#include <swift2d/properties.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class SWIFT_DLL FPSCounter {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  FPSCounter(unsigned t = 10);

  // Call this after creation of this counter.
  void start();

  // Call this once a frame.
  void step();

  // This property contains the current frames per seconds.
  Float fps;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  unsigned frame_count;
  Timer    timer;
  unsigned delay;
};

}

#endif  // SWIFT2D_FPS_COUNTER_HPP
