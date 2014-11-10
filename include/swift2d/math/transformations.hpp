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
#include <swift2d/math/types.hpp>
#include <swift2d/utils/platform.hpp>

namespace swift {
namespace math {

SWIFT_DLL mat3 make_scale(vec2 const& scale);
SWIFT_DLL mat3 make_scale(float scale);
SWIFT_DLL mat3 make_scale(float x, float y);
SWIFT_DLL mat3 make_rotation(float angle);
SWIFT_DLL mat3 make_translation(vec2 const& delta);
SWIFT_DLL mat3 make_translation(float x, float y);
SWIFT_DLL mat3 make_inversed(mat3 const& mat);
SWIFT_DLL mat3 make_transposed(mat3 const& mat);

SWIFT_DLL void scale(mat3& mat, vec2 const& scale);
SWIFT_DLL void scale(mat3& mat, float scale);
SWIFT_DLL void scale(mat3& mat, float x, float y);
SWIFT_DLL void rotate(mat3& mat, float angle);
SWIFT_DLL void translate(mat3& mat, vec2 const& delta);
SWIFT_DLL void translate(mat3& mat, float x, float y);
SWIFT_DLL void inverse(mat3& mat);
SWIFT_DLL void transpose(mat3& mat);

SWIFT_DLL mat3 scaled(mat3 const& mat, vec2 const& scale);
SWIFT_DLL mat3 scaled(mat3 const& mat, float scale);
SWIFT_DLL mat3 scaled(mat3 const& mat, float x, float y);
SWIFT_DLL mat3 rotated(mat3 const& mat, float angle);
SWIFT_DLL mat3 translated(mat3 const& mat, vec2 const& delta);
SWIFT_DLL mat3 translated(mat3 const& mat, float x, float y);
SWIFT_DLL mat3 inversed(mat3 const& mat);
SWIFT_DLL mat3 transposed(mat3 const& mat);

SWIFT_DLL vec2  get_translation(mat3 const& mat);
SWIFT_DLL vec2  get_scale(mat3 const& mat);
SWIFT_DLL float get_rotation(mat3 const& mat);
SWIFT_DLL vec2  get_direction(mat3 const& mat);

SWIFT_DLL void set_translation(mat3& mat, vec2 const& val);
SWIFT_DLL void set_translation(mat3& mat, float x, float y);
SWIFT_DLL void set_scale(mat3& mat, vec2 const& val);
SWIFT_DLL void set_rotation(mat3& mat, float val);

SWIFT_DLL vec3 normalized(vec3 const& vec);
SWIFT_DLL vec2 normalized(vec2 const& vec);

SWIFT_DLL void normalize(vec3& vec);
SWIFT_DLL void normalize(vec2& vec);

SWIFT_DLL float dot(vec2 const& a, vec2 const& b);
SWIFT_DLL float dot(vec3 const& a, vec3 const& b);

SWIFT_DLL mat4 to_3D(mat3 const& mat);

}
}

#endif // SWIFT2D_MATH_TRANSFORMATIONS_HPP
