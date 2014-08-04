////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_HPP
#define SWIFT2D_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Logger.hpp>
#include <swift2d/utils/stl_helpers.hpp>
#include <swift2d/math.hpp>
#include <swift2d/events.hpp>
#include <swift2d/properties.hpp>
#include <swift2d/scene.hpp>
#include <swift2d/components.hpp>
#include <swift2d/behaviors.hpp>
#include <swift2d/shapes.hpp>
#include <swift2d/renderer.hpp>
#include <swift2d/databases.hpp>
#include <swift2d/materials.hpp>
#include <swift2d/particles.hpp>
#include <swift2d/trails.hpp>
#include <swift2d/physics.hpp>
#include <swift2d/input/keyboard_enums.hpp>
#include <swift2d/input/mouse_enums.hpp>
#include <swift2d/input/joystick_enums.hpp>
#include <swift2d/triggers/ShapeTrigger.hpp>
#include <swift2d/application/Application.hpp>
#include <swift2d/network.hpp>
#include <swift2d/audio.hpp>
#include <swift2d/gui.hpp>
#include <swift2d/steam/Steam.hpp>

namespace swift {

/**
 * Initialize swift2d.
 *
 * This should be called once at the beginning of every application
 * using swift2d.
 */

void init(int argc, char** argv);
void clean_up();

}

#endif  // SWIFT2D_HPP

