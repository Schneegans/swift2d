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

// includes  -------------------------------------------------------------------
#include <swift2d/utils/FPSCounter.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

FPSCounter::FPSCounter(unsigned t)
  : Fps(0.0f)
  , frame_count(0)
  , timer()
  , delay(t) {}

////////////////////////////////////////////////////////////////////////////////

void FPSCounter::start() {
  timer.start();
}

////////////////////////////////////////////////////////////////////////////////

void FPSCounter::step() {
  if (++frame_count == delay) {
    Fps = 1.f * delay / float(timer.get_elapsed());
    timer.reset();
    frame_count = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////

}
