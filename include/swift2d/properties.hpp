////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_PROPERTIES_HPP
#define SWIFT2D_EVENTS_PROPERTIES_HPP

#include <swift2d/properties/Property.hpp>
#include <swift2d/properties/AnimatedProperty.hpp>
#include <swift2d/math/types.hpp>

#include <string>

namespace swift {

typedef Property<double>         Double;
typedef Property<float>          Float;
typedef Property<int>            Int;
typedef Property<char>           Char;
typedef Property<unsigned>       UInt;
typedef Property<unsigned char>  UChar;

typedef Property<math::vec3>     Vec3;
typedef Property<math::vec2i>    Vec2i;
typedef Property<math::vec2>     Vec2;
typedef Property<math::mat3>     Mat3;

typedef Property<bool>           Bool;

typedef Property<std::string>    String;


typedef AnimatedProperty<double>        AnimatedDouble;
typedef AnimatedProperty<float>         AnimatedFloat;
typedef AnimatedProperty<math::vec2>    AnimatedVec2;

}

#endif /* SWIFT2D_EVENTS_PROPERTIES_HPP */
