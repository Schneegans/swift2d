////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MOVER_HPP
#define MOVER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

using namespace swift;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Mover : public MoveBehavior {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Mover() {
    auto window = WindowManager::get().current();
    connection_ = window->on_key_press.connect([&](Key key, int scancode, int action, int mods){
      if (action == 0) {
        if (key == Key::W) LinearSpeed.set(0);
        if (key == Key::S) LinearSpeed.set(0);
        if (key == Key::A) AngularSpeed.set(0);
        if (key == Key::D) AngularSpeed.set(0);
      } else if (action == 1) {
        if (key == Key::W) LinearSpeed.set( 10);
        if (key == Key::S) LinearSpeed.set(-10);
        if (key == Key::A) AngularSpeed.set(-2 );
        if (key == Key::D) AngularSpeed.set( 2 );
      }
    });
  }

  ~Mover() {
    auto window = WindowManager::get().current();
    window->on_key_press.disconnect(connection_);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  int connection_;

};

#endif  // MOVER_HPP
