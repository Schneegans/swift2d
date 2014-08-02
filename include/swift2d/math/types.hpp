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
#include <swift2d/opengl.hpp>

namespace swift {
namespace math {

// typedefs  -------------------------------------------------------------------
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
