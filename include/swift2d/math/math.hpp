////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATH_HPP
#define SWIFT2D_MATH_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/opengl.hpp>
#include <swift2d/math/stream.hpp>

#include <glm/glm.hpp>


namespace swift {
namespace math {

typedef oglplus::Mat3f mat3;

// Methods for matrix creation
glm::mat4 make_scale(glm::vec3 const& scale);
glm::mat4 make_scale(float scale);
glm::mat4 make_scale(float x, float y, float z);
glm::mat4 make_translate(glm::vec3 const& delta);
glm::mat4 make_translate(glm::vec4 const& delta);
glm::mat4 make_translate(float x, float y, float z);
glm::mat4 make_rotate(float angle, glm::vec3 const& axis);
glm::mat4 make_rotate(float angle, float x, float y, float z);
glm::mat4 make_perspective(
  glm::vec4 const& eye_position,
  glm::mat4 const& screen_transform,
  float near_plane,
  float far_plane
);
glm::mat4 make_orthographic(
  glm::vec4 const& eye_position,
  glm::mat4 const& screen_transform,
  float near_plane,
  float far_plane
);

}}

#endif // SWIFT2D_MATH_HPP
