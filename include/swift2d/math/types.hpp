////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATH_TYPES_HPP
#define SWIFT2D_MATH_TYPES_HPP

// includes  -------------------------------------------------------------------
#include <GL/glew.h>
#include <oglplus/fwd.hpp>
#include <oglplus/math/matrix.hpp>

namespace swift {
namespace math {

// typedefs  -------------------------------------------------------------------

// same typedefs as in steam api
typedef char int8;
typedef unsigned char uint8;

#if defined( _WIN32 )
  typedef __int16 int16;
  typedef unsigned __int16 uint16;
  typedef __int32 int32;
  typedef unsigned __int32 uint32;
  typedef __int64 int64;
  typedef unsigned __int64 uint64;

  typedef int64 lint64;
  typedef uint64 ulint64;
#else // _WIN32
  typedef short int16;
  typedef unsigned short uint16;
  typedef int int32;
  typedef unsigned int uint32;
  typedef long long int64;
  typedef unsigned long long uint64;

  // [u]int64 are actually defined as 'long long' and gcc 64-bit
  // doesn't automatically consider them the same as 'long int'.
  // Changing the types for [u]int64 is complicated by
  // there being many definitions, so we just
  // define a 'long int' here and use it in places that would
  // otherwise confuse the compiler.
  typedef long int lint64;
  typedef unsigned long int ulint64;
#endif // else _WIN32

typedef oglplus::Matrix<float, 2, 2> mat2;
typedef oglplus::Matrix<float, 3, 3> mat3;
typedef oglplus::Matrix<float, 4, 4> mat4;

typedef oglplus::Vector<float, 2> vec2;
typedef oglplus::Vector<float, 3> vec3;
typedef oglplus::Vector<float, 4> vec4;

typedef oglplus::Vector<int, 2> vec2i;
typedef oglplus::Vector<int, 3> vec3i;
typedef oglplus::Vector<int, 4> vec4i;

typedef oglplus::Vector<unsigned, 2> vec2ui;
typedef oglplus::Vector<unsigned, 3> vec3ui;
typedef oglplus::Vector<unsigned, 4> vec4ui;

}
}

#endif // SWIFT2D_MATH_TYPES_HPP
