////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_PROPERTIES_HPP
#define SWIFT2D_EVENTS_PROPERTIES_HPP

#include <swift2d/properties/Property.hpp>
#include <swift2d/properties/AnimatedProperty.hpp>

#include <raknet/RakString.h>
#include <string>

namespace swift {

typedef SWIFT_DLL Property<double>              Double;
typedef SWIFT_DLL Property<float>               Float;
typedef SWIFT_DLL Property<math::int8>          Int8;
typedef SWIFT_DLL Property<math::int16>         Int16;
typedef SWIFT_DLL Property<math::int32>         Int32;
typedef SWIFT_DLL Property<math::int64>         Int64;
typedef SWIFT_DLL Property<math::uint8>         UInt8;
typedef SWIFT_DLL Property<math::uint16>        UInt16;
typedef SWIFT_DLL Property<math::uint32>        UInt32;
typedef SWIFT_DLL Property<math::uint64>        UInt64;

typedef SWIFT_DLL Property<math::vec3>          Vec3;
typedef SWIFT_DLL Property<math::vec3i>         Vec3i;
typedef SWIFT_DLL Property<math::vec3ui>        Vec3ui;
typedef SWIFT_DLL Property<math::vec2>          Vec2;
typedef SWIFT_DLL Property<math::vec2i>         Vec2i;
typedef SWIFT_DLL Property<math::vec2ui>        Vec2ui;
typedef SWIFT_DLL Property<math::mat3>          Mat3;

typedef SWIFT_DLL Property<bool>                Bool;

typedef SWIFT_DLL Property<std::string>         String;
typedef SWIFT_DLL Property<RakNet::RakString>   NetString;

}

#endif /* SWIFT2D_EVENTS_PROPERTIES_HPP */
