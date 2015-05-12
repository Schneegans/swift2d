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

  // ---------------------------------------------------------------- properties

  // This property contains the current frames per seconds.
  Float Fps;

  // ----------------------------------------------------- contruction interface

  // Every t frames the Fps property is updated.
  FPSCounter(unsigned t = 10);

  // ------------------------------------------------------------ public methods

  // Call this after creation of this counter.
  void start();

  // Call this once a frame.
  void step();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  unsigned frame_count;
  Timer    timer;
  unsigned delay;
};

}

#endif  // SWIFT2D_FPS_COUNTER_HPP
