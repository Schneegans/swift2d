////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/input/joystick_enums.hpp>

namespace swift {

bool operator== (JoystickButtonId joy_button, Xbox360ControllerButton xbox_button) {
  return static_cast<int>(joy_button) == static_cast<int>(xbox_button);
}

bool operator== (JoystickAxisId joy_axis, Xbox360ControllerAxis xbox_axis) {
  return static_cast<int>(joy_axis) == static_cast<int>(xbox_axis);
}

}
