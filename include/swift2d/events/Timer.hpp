////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TIMER_HPP
#define SWIFT2D_TIMER_HPP

#include <string>

namespace swift {

class Timer {

 public:
  void start();
  void reset();

  double get_elapsed() const;

  static double get_now();

 private:
  double start_;

};

}

#endif  // SWIFT2D_TIMER_HPP
