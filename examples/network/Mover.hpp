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
    auto window = WindowManager::instance()->get_default();
    connection_ = window->on_key_press.connect([&](Key key, int scancode, int action, int mods){
      if (action == 0) {
        if (key == Key::W) LinearSpeed.set(0, 0.5);
        if (key == Key::S) LinearSpeed.set(0, 0.5);
        if (key == Key::A) AngularSpeed.set(0, 0.1);
        if (key == Key::D) AngularSpeed.set(0, 0.1);
      } else if (action == 1) {
        if (key == Key::W) LinearSpeed.set( 20, 1);
        if (key == Key::S) LinearSpeed.set(-20, 1);
        if (key == Key::A) AngularSpeed.set(-2 , 0.5);
        if (key == Key::D) AngularSpeed.set( 2 , 0.5);
      }
    });
  }

  ~Mover() {
    auto window = WindowManager::instance()->get_default();
    window->on_key_press.disconnect(connection_);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  int connection_;

};

#endif  // MOVER_HPP
