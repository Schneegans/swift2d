////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_JOYSTICK_ENUMS_HPP
#define SWIFT2D_JOYSTICK_ENUMS_HPP


namespace swift {

enum JoystickId {
  JOYSTICK_0   =  0,
  JOYSTICK_1   =  1,
  JOYSTICK_2   =  2,
  JOYSTICK_3   =  3,
  JOYSTICK_4   =  4,
  JOYSTICK_5   =  5,
  JOYSTICK_6   =  6,
  JOYSTICK_7   =  7,
  JOYSTICK_8   =  8,
  JOYSTICK_9   =  9,
  JOYSTICK_10  = 10,
  JOYSTICK_11  = 11,
  JOYSTICK_12  = 12,
  JOYSTICK_13  = 13,
  JOYSTICK_14  = 14,
  JOYSTICK_15  = 15,
  JOYSTICK_NUM = 16
};

enum JoystickAxisId {
  JOYSTICK_AXIS_0   =  0,
  JOYSTICK_AXIS_1   =  1,
  JOYSTICK_AXIS_2   =  2,
  JOYSTICK_AXIS_3   =  3,
  JOYSTICK_AXIS_4   =  4,
  JOYSTICK_AXIS_5   =  5,
  JOYSTICK_AXIS_6   =  6,
  JOYSTICK_AXIS_7   =  7,
  JOYSTICK_AXIS_8   =  8,
  JOYSTICK_AXIS_9   =  9,
  JOYSTICK_AXIS_10  = 10,
  JOYSTICK_AXIS_11  = 11,
  JOYSTICK_AXIS_12  = 12,
  JOYSTICK_AXIS_13  = 13,
  JOYSTICK_AXIS_14  = 14,
  JOYSTICK_AXIS_15  = 15,
  JOYSTICK_AXIS_16  = 16,
  JOYSTICK_AXIS_17  = 17,
  JOYSTICK_AXIS_18  = 18,
  JOYSTICK_AXIS_19  = 19,
  JOYSTICK_AXIS_20  = 20,
  JOYSTICK_AXIS_21  = 21,
  JOYSTICK_AXIS_22  = 22,
  JOYSTICK_AXIS_23  = 23,
  JOYSTICK_AXIS_24  = 24,
  JOYSTICK_AXIS_25  = 25,
  JOYSTICK_AXIS_26  = 26,
  JOYSTICK_AXIS_27  = 27,
  JOYSTICK_AXIS_28  = 28,
  JOYSTICK_AXIS_29  = 29,
  JOYSTICK_AXIS_30  = 30,
  JOYSTICK_AXIS_31  = 31,
  JOYSTICK_AXIS_NUM = 32
};

enum JoystickButtonId {
  JOYSTICK_BUTTON_0   =  0,
  JOYSTICK_BUTTON_1   =  1,
  JOYSTICK_BUTTON_2   =  2,
  JOYSTICK_BUTTON_3   =  3,
  JOYSTICK_BUTTON_4   =  4,
  JOYSTICK_BUTTON_5   =  5,
  JOYSTICK_BUTTON_6   =  6,
  JOYSTICK_BUTTON_7   =  7,
  JOYSTICK_BUTTON_8   =  8,
  JOYSTICK_BUTTON_9   =  9,
  JOYSTICK_BUTTON_10  = 10,
  JOYSTICK_BUTTON_11  = 11,
  JOYSTICK_BUTTON_12  = 12,
  JOYSTICK_BUTTON_13  = 13,
  JOYSTICK_BUTTON_14  = 14,
  JOYSTICK_BUTTON_15  = 15,
  JOYSTICK_BUTTON_16  = 16,
  JOYSTICK_BUTTON_17  = 17,
  JOYSTICK_BUTTON_18  = 18,
  JOYSTICK_BUTTON_19  = 19,
  JOYSTICK_BUTTON_20  = 20,
  JOYSTICK_BUTTON_21  = 21,
  JOYSTICK_BUTTON_22  = 22,
  JOYSTICK_BUTTON_23  = 23,
  JOYSTICK_BUTTON_24  = 24,
  JOYSTICK_BUTTON_25  = 25,
  JOYSTICK_BUTTON_26  = 26,
  JOYSTICK_BUTTON_27  = 27,
  JOYSTICK_BUTTON_28  = 28,
  JOYSTICK_BUTTON_29  = 29,
  JOYSTICK_BUTTON_30  = 30,
  JOYSTICK_BUTTON_31  = 31,
  JOYSTICK_BUTTON_NUM = 32
};

enum Xbox360ControllerButton {
  A                  =  JoystickButtonId::JOYSTICK_BUTTON_0,
  B                  =  JoystickButtonId::JOYSTICK_BUTTON_1,
  X                  =  JoystickButtonId::JOYSTICK_BUTTON_2,
  Y                  =  JoystickButtonId::JOYSTICK_BUTTON_3,
  BUMPER_LEFT        =  JoystickButtonId::JOYSTICK_BUTTON_4,
  BUMPER_RIGHT       =  JoystickButtonId::JOYSTICK_BUTTON_5,
  BACK               =  JoystickButtonId::JOYSTICK_BUTTON_6,
  START              =  JoystickButtonId::JOYSTICK_BUTTON_7,
  XBOX               =  JoystickButtonId::JOYSTICK_BUTTON_8,
  LEFT_STICK_BUTTON  =  JoystickButtonId::JOYSTICK_BUTTON_9,
  RIGHT_STICK_BUTTON = JoystickButtonId::JOYSTICK_BUTTON_10,
};

enum Xbox360ControllerAxis {
  LEFT_STICK_X       =      JoystickAxisId::JOYSTICK_AXIS_0,
  LEFT_STICK_Y       =      JoystickAxisId::JOYSTICK_AXIS_1,
  LEFT_TRIGGER       =      JoystickAxisId::JOYSTICK_AXIS_2,
  RIGHT_STICK_X      =      JoystickAxisId::JOYSTICK_AXIS_3,
  RIGHT_STICK_Y      =      JoystickAxisId::JOYSTICK_AXIS_4,
  RIGHT_TRIGGER      =      JoystickAxisId::JOYSTICK_AXIS_5,
  DIRECTIONAL_PAD_X  =      JoystickAxisId::JOYSTICK_AXIS_6,
  DIRECTIONAL_PAD_Y  =      JoystickAxisId::JOYSTICK_AXIS_7
};

bool operator== (JoystickButtonId joy_button, Xbox360ControllerButton xbox_button);
bool operator== (JoystickAxisId joy_axis, Xbox360ControllerAxis xbox_axis);

}

#endif // SWIFT2D_JOYSTICK_ENUMS_HPP
