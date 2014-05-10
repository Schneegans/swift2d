////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATH_TRANSFORMATIONS_HPP
#define SWIFT2D_MATH_TRANSFORMATIONS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/opengl.hpp>
#include <swift2d/math/types.hpp>

namespace swift {
namespace math {

mat3 make_scale     (vec2 const& scale);
mat3 make_scale     (float scale);
mat3 make_scale     (float x, float y);
mat3 make_rotate    (float angle);
mat3 make_translate (vec2 const& delta);
mat3 make_translate (float x, float y);

void scale     (mat3& mat, vec2 const& scale);
void scale     (mat3& mat, float scale);
void scale     (mat3& mat, float x, float y);
void rotate    (mat3& mat, float angle);
void translate (mat3& mat, vec2 const& delta);
void translate (mat3& mat, float x, float y);

mat3 scaled     (mat3 const& mat, vec2 const& scale);
mat3 scaled     (mat3 const& mat, float scale);
mat3 scaled     (mat3 const& mat, float x, float y);
mat3 rotated    (mat3 const& mat, float angle);
mat3 translated (mat3 const& mat, vec2 const& delta);
mat3 translated (mat3 const& mat, float x, float y);

vec2 get_position (mat3 const& mat);

}
}

#endif // SWIFT2D_MATH_TRANSFORMATIONS_HPP