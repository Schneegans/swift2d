////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_CAMERA_HPP
#define SWIFT2D_CAMERA_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/NodePath.hpp>
#include <swift2d/utils/configuration_macro.hpp>

#include <string>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// This struct describes a user's view on the scene. It is defined by a       //
// screen, a view point a a render mask.                                      //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
struct Camera {

  enum ProjectionMode {
    PERSPECTIVE,
    ORTHOGRAPHIC
  };

  SWIFT2D_ADD_VALUE(Camera, NodePath, eye_left, NodePath());
  SWIFT2D_ADD_VALUE(Camera, NodePath, eye_right, NodePath());
  SWIFT2D_ADD_VALUE(Camera, NodePath, screen_left, NodePath());
  SWIFT2D_ADD_VALUE(Camera, NodePath, screen_right, NodePath());

  SWIFT2D_ADD_VALUE(Camera, std::string, scene, "scene");
  SWIFT2D_ADD_VALUE(Camera, std::string, mask, "");

  SWIFT2D_ADD_VALUE(Camera, ProjectionMode, mode, ProjectionMode::PERSPECTIVE);
};

}

#endif // SWIFT2D_CAMERA_HPP
