////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_PROPERTIES_HPP
#define SWIFT2D_EVENTS_PROPERTIES_HPP

#include <swift2d/events/properties/NumericProperty.hpp>
#include <swift2d/events/properties/LogicalProperty.hpp>
#include <swift2d/math/math.hpp>

#include <string>

namespace swift {

typedef NumericProperty<double>         Double;
typedef NumericProperty<float>          Float;
typedef NumericProperty<int>            Int;
typedef NumericProperty<char>           Char;
typedef NumericProperty<unsigned>       UInt;
typedef NumericProperty<unsigned char>  UChar;

typedef NumericProperty<glm::vec3>      Vec3;
typedef NumericProperty<glm::vec2>      Vec2;
typedef NumericProperty<glm::mat4>      Mat4;

typedef LogicalProperty<bool>           Bool;

typedef StreamableProperty<std::string> String;
}

#endif /* SWIFT2D_EVENTS_PROPERTIES_HPP */
