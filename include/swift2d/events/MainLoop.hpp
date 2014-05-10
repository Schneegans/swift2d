////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_MAINLOOP_HPP
#define SWIFT2D_EVENTS_MAINLOOP_HPP

#include <functional>

namespace swift {

class MainLoop {
 public:
  MainLoop();
  ~MainLoop();

  void start();
  void stop();

};

}

#endif /* SWIFT2D_EVENTS_MAINLOOP_HPP */
