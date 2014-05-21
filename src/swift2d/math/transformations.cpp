////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/math/transformations.hpp>

#include <iostream>
#include <sstream>

namespace swift {
namespace math {

////////////////////////////////////////////////////////////////////////////////

mat3 make_scale (vec2 const& scale) {
  return make_scale(scale.x(), scale.y());
}

////////////////////////////////////////////////////////////////////////////////

mat3 make_scale (float scale) {
  return make_scale(scale, scale);
}

////////////////////////////////////////////////////////////////////////////////

mat3 make_scale (float x, float y) {
  return mat3(x, 0, 0,
              0, y, 0,
              0, 0, 1);
}

////////////////////////////////////////////////////////////////////////////////

mat3 make_rotate (float angle) {
  return mat3( std::cos(angle), std::sin(angle), 0,
              -std::sin(angle), std::cos(angle), 0,
               0,               0,               1);
}

////////////////////////////////////////////////////////////////////////////////

mat3 make_translate (vec2 const& delta) {
  return make_translate(delta.x(), delta.y());
}

////////////////////////////////////////////////////////////////////////////////

mat3 make_translate (float x, float y) {
  return mat3(1, 0, x,
              0, 1, y,
              0, 0, 1);
}

////////////////////////////////////////////////////////////////////////////////

mat3 make_inversed (mat3 const& mat) {
  return oglplus::Inverse(mat);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void scale (mat3& mat, vec2 const& scale) {
  mat = mat * make_scale(scale);
}

////////////////////////////////////////////////////////////////////////////////

void scale (mat3& mat, float scale) {
  mat = mat * make_scale(scale);
}

////////////////////////////////////////////////////////////////////////////////

void scale (mat3& mat, float x, float y) {
  mat = mat * make_scale(x, y);
}

////////////////////////////////////////////////////////////////////////////////

void rotate (mat3& mat, float angle) {
  mat = mat * make_rotate(angle);
}

////////////////////////////////////////////////////////////////////////////////

void translate (mat3& mat, vec2 const& delta) {
  mat = mat * make_translate(delta);
}

////////////////////////////////////////////////////////////////////////////////

void translate (mat3& mat, float x, float y) {
  mat = mat * make_translate(x, y);
}

////////////////////////////////////////////////////////////////////////////////

void inverse (mat3& mat) {
  mat = make_inversed(mat);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

mat3 scaled (mat3 const& mat, vec2 const& scale) {
  return mat * make_scale(scale);
}

////////////////////////////////////////////////////////////////////////////////

mat3 scaled (mat3 const& mat, float scale) {
  return mat * make_scale(scale);
}

////////////////////////////////////////////////////////////////////////////////

mat3 scaled (mat3 const& mat, float x, float y) {
  return mat * make_scale(x, y);
}

////////////////////////////////////////////////////////////////////////////////

mat3 rotated (mat3 const& mat, float angle) {
  return mat * make_rotate(angle);
}

////////////////////////////////////////////////////////////////////////////////

mat3 translated (mat3 const& mat, vec2 const& delta) {
  return mat * make_translate(delta);
}

////////////////////////////////////////////////////////////////////////////////

mat3 translated (mat3 const& mat, float x, float y) {
  return mat * make_translate(x, y);
}

////////////////////////////////////////////////////////////////////////////////

mat3 inversed (mat3 const& mat) {
  return make_inversed(mat);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

vec2 get_translate (mat3 const& mat) {
  return vec2(mat.At(0, 2), mat.At(1, 2));
}

////////////////////////////////////////////////////////////////////////////////

vec2 get_scale (mat3 const& mat) {
  return vec2((vec2(mat.At(0, 0), mat.At(0, 1))).Length(),
              (vec2(mat.At(1, 0), mat.At(1, 1))).Length());
}

////////////////////////////////////////////////////////////////////////////////

float get_rotation (mat3 const& mat) {
  float tmp(-std::atan2(mat.At(0, 0), mat.At(0, 1)) + M_PI/2);
  return tmp >= 0 ? tmp : tmp + 2*M_PI;
}

////////////////////////////////////////////////////////////////////////////////

}
}
