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

typedef SWIFT_DLL Property<double>         Double;
typedef SWIFT_DLL Property<float>          Float;
typedef SWIFT_DLL Property<int>            Int;
typedef SWIFT_DLL Property<char>           Char;
typedef SWIFT_DLL Property<short>          Short;
typedef SWIFT_DLL Property<unsigned>       UInt;
typedef SWIFT_DLL Property<unsigned char>  UChar;

typedef SWIFT_DLL Property<math::vec3>     Vec3;
typedef SWIFT_DLL Property<math::vec2i>    Vec2i;
typedef SWIFT_DLL Property<math::vec2>     Vec2;
typedef SWIFT_DLL Property<math::mat3>     Mat3;

typedef SWIFT_DLL Property<bool>           Bool;

typedef SWIFT_DLL Property<std::string>    String;

typedef SWIFT_DLL AnimatedProperty<double>        AnimatedDouble;
typedef SWIFT_DLL AnimatedProperty<float>         AnimatedFloat;
typedef SWIFT_DLL AnimatedProperty<math::vec2>    AnimatedVec2;

}

#endif /* SWIFT2D_EVENTS_PROPERTIES_HPP */
