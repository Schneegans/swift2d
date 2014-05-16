/******************************************************************************
 * guacamole - delicious VR                                                   *
 *                                                                            *
 * Copyright: (c) 2011-2013 Bauhaus-Universität Weimar                        *
 * Contact:   felix.lauer@uni-weimar.de / simon.schneegans@uni-weimar.de      *
 *                                                                            *
 * This program is free software: you can redistribute it and/or modify it    *
 * under the terms of the GNU General Public License as published by the Free *
 * Software Foundation, either version 3 of the License, or (at your option)  *
 * any later version.                                                         *
 *                                                                            *
 * This program is distributed in the hope that it will be useful, but        *
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   *
 * for more details.                                                          *
 *                                                                            *
 * You should have received a copy of the GNU General Public License along    *
 * with this program. If not, see <http://www.gnu.org/licenses/>.             *
 *                                                                            *
 ******************************************************************************/

#ifndef SWIFT2D_FPS_COUNTER_HPP
#define SWIFT2D_FPS_COUNTER_HPP

#include <swift2d/events/Timer.hpp>
#include <swift2d/properties.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class FPSCounter {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  FPSCounter(unsigned t) : fps(0.0f), frame_count(0), timer(), delay(t) {}

  // Call this after creation of this counter.
  void start() { timer.start(); }

  // Call this once a frame.
  void step() {
    if (++frame_count == delay) {
      fps = 1.f * delay / float(timer.get_elapsed());
      timer.reset();
      frame_count = 0;
    }
  }

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
