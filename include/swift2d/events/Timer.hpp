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

#ifndef SWIFT2D_TIMER_HPP
#define SWIFT2D_TIMER_HPP

// includes  ------------------------------------------------------------------
#include <swift2d/utils/platform.hpp>

#include <string>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A class which can be used to measure time intervals.                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Timer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  Timer();

  void   start();
  double reset();

  double get_elapsed() const;
  bool   is_running() const;

  static double get_now();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  double start_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_TIMER_HPP
