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
typedef oglplus::Mat2f mat2;
typedef oglplus::Mat3f mat3;
typedef oglplus::Mat4f mat4;

typedef oglplus::Vec2f vec2;
typedef oglplus::Vec3f vec3;
typedef oglplus::Vec4f vec4;

typedef oglplus::Vector<GLint, 2> vec2i;
typedef oglplus::Vector<GLint, 3> vec3i;
typedef oglplus::Vector<GLint, 4> vec4i;

typedef oglplus::Vector<GLuint, 2> vec2ui;
typedef oglplus::Vector<GLuint, 3> vec3ui;
typedef oglplus::Vector<GLuint, 4> vec4ui;

}
}

#endif // SWIFT2D_MATH_TYPES_HPP
