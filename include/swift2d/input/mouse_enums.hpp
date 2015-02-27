////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MOUSE_ENUMS_HPP
#define SWIFT2D_MOUSE_ENUMS_HPP


namespace swift {

enum class Button : int {
  BUTTON_1     =   0,
  BUTTON_2     =   1,
  BUTTON_3     =   2,
  BUTTON_4     =   3,
  BUTTON_5     =   4,
  BUTTON_6     =   5,
  BUTTON_7     =   6,
  BUTTON_8     =   7
};

enum class Cursor : int {
  POINTER,
  CROSS,
  HAND,
  IBEAM,
  WAIT,
  HELP,
  EAST_RESIZE,
  NORTH_RESIZE,
  NORTH_EAST_RESIZE,
  NORTH_WEST_RESIZE,
  SOUTH_RESIZE,
  SOUTH_EAST_RESIZE,
  SOUTH_WEST_RESIZE,
  WEST_RESIZE,
  NORTH_SOUTH_RESIZE,
  EAST_WEST_RESIZE,
  NORTH_EAST_SOUTH_WEST_RESIZE,
  NORTH_WEST_SOUTH_EAST_RESIZE,
  COLUMN_RESIZE,
  ROW_RESIZE,
  MIDDLE_PANNING,
  EAST_PANNING,
  NORTH__PANNING,
  NORTH_EAST_PANNING,
  NORTH_WEST_PANNING,
  SOUTH__PANNING,
  SOUTH_EAST_PANNING,
  SOUTH_WEST_PANNING,
  WEST_PANNING,
  MOVE,
  VERTICAL_TEXT,
  CELL,
  CONTEXT_MENU,
  ALIAS,
  PROGRESS,
  NO_DROP,
  COPY,
  NONE,
  NOT_ALLOWED,
  ZOOM_IN,
  ZOOM_OUT,
  GRAB,
  GRABBING,
  CUSTOM
};


}

#endif // SWIFT2D_MOUSE_ENUMS_HPP
