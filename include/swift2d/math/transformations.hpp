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

#ifndef SWIFT2D_MATH_TRANSFORMATIONS_HPP
#define SWIFT2D_MATH_TRANSFORMATIONS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/math/types.hpp>
#include <swift2d/utils/platform.hpp>

namespace swift {
namespace math {

SWIFT_DLL mat3 make_transformation(vec2 const& pos, float rot, float scale);
SWIFT_DLL mat3 make_transformation(vec2 const& pos, float rot, vec2 const& scale);

SWIFT_DLL mat3 make_scale(vec2 const& scale);
SWIFT_DLL mat3 make_scale(float scale);
SWIFT_DLL mat3 make_scale(float x, float y);
SWIFT_DLL mat3 make_rotation(float angle);
SWIFT_DLL mat3 make_rotation(vec2 const& normalized_direction);
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

SWIFT_DLL vec2 rotated(vec2 const& vector, float angle);

SWIFT_DLL vec2  get_translation(mat3 const& mat);
SWIFT_DLL vec2  get_scale(mat3 const& mat);
SWIFT_DLL float get_rotation(mat3 const& mat);
SWIFT_DLL vec2  get_direction(mat3 const& mat);
SWIFT_DLL vec2  get_direction(float rotation);

SWIFT_DLL float get_rotation(vec2 const& vec);
SWIFT_DLL float get_length(vec2 const& vec);
SWIFT_DLL float get_length_squared(vec2 const& vec);

SWIFT_DLL float get_angle(vec2 const& normalized_a, vec2 const& normalized_b);
SWIFT_DLL float get_cw_angle(vec2 const& normalized_a, vec2 const& normalized_b);
SWIFT_DLL float get_ccw_angle(vec2 const& normalized_a, vec2 const& normalized_b);

SWIFT_DLL bool get_cw(vec2 const& a, vec2 const& b);
SWIFT_DLL bool get_ccw(vec2 const& a, vec2 const& b);

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

SWIFT_DLL vec3 cross(vec3 const& a, vec3 const& b);

SWIFT_DLL mat4  to_3D(mat3 const& mat);


}
}

#endif // SWIFT2D_MATH_TRANSFORMATIONS_HPP
