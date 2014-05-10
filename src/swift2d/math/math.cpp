////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/math/math.hpp>

#include <iostream>
#include <sstream>

namespace swift {
namespace math {


////////////////////////////////////////////////////////////////////////////////

glm::mat4 make_scale(glm::vec3 const& scale) {
    return glm::scale(glm::mat4(1.f), scale);
}

////////////////////////////////////////////////////////////////////////////////

glm::mat4 make_scale(float scale) {
    return glm::scale(glm::mat4(1.f), glm::vec3(scale, scale, scale));
}

////////////////////////////////////////////////////////////////////////////////

glm::mat4 make_scale(float x, float y, float z) {
    return make_scale(glm::vec3(x, y, z));
}

////////////////////////////////////////////////////////////////////////////////

glm::mat4 make_translate(glm::vec3 const& delta) {
    return glm::translate(glm::mat4(1.f), delta);
}

////////////////////////////////////////////////////////////////////////////////

glm::mat4 make_translate(glm::vec4 const& delta) {
    return glm::translate(glm::mat4(1.f), glm::swizzle<glm::X, glm::Y, glm::Z>(delta));
}

////////////////////////////////////////////////////////////////////////////////

glm::mat4 make_translate(float x, float y, float z) {
    return make_translate(glm::vec3(x, y, z));
}

////////////////////////////////////////////////////////////////////////////////

glm::mat4 make_rotate(float angle, glm::vec3 const& axis) {
    return glm::rotate(glm::mat4(1.f), angle, axis);
}

////////////////////////////////////////////////////////////////////////////////

glm::mat4 make_rotate(float angle, float x, float y, float z) {
    return make_rotate(angle, glm::vec3(x, y, z));
}

////////////////////////////////////////////////////////////////////////////////

glm::mat4 make_perspective(glm::vec4 const& eye_position,
                           glm::mat4 const& screen_transform,
                           float near_plane,
                           float far_plane) {

  glm::vec4 relative_eye_position(glm::inverse(screen_transform) *
                                   eye_position);

  float d(relative_eye_position[2]);
  float ox(-relative_eye_position[0]);
  float oy(-relative_eye_position[1]);

  glm::mat4 frustum(glm::mat4(1.f));

  // frustum[0] = 2 * d;
  // frustum[5] = 2 * d;
  // frustum[8] = 2 * ox;
  // frustum[9] = 2 * oy;
  // frustum[10] = (-near_plane - far_plane) / (far_plane - near_plane);
  // frustum[11] = -1.f;
  // frustum[14] = -2 * near_plane * far_plane / (far_plane - near_plane);
  // frustum[15] = 0.f;

  frustum[0][0] = 2 * d;
  frustum[1][1] = 2 * d;
  frustum[2][0] = 2 * ox;
  frustum[2][1] = 2 * oy;
  frustum[2][2] = (-near_plane - far_plane) / (far_plane - near_plane);
  frustum[2][3] = -1.f;
  frustum[3][2] = -2 * near_plane * far_plane / (far_plane - near_plane);
  frustum[3][3] = 0.f;

  return frustum;
}

////////////////////////////////////////////////////////////////////////////////

glm::mat4 make_orthographic(glm::vec4 const& eye_position,
                            glm::mat4 const& screen_transform,
                            float near_plane,
                            float far_plane) {

  glm::vec4 relative_eye_position(glm::inverse(screen_transform) *
                                   eye_position);

  float ox(-relative_eye_position[0]);
  float oy(-relative_eye_position[1]);

  glm::mat4 frustum(glm::mat4(1.f));

  // frustum[0] = 2.0f;
  // frustum[5] = 2.0f;
  // frustum[10] = 2.0f / (near_plane - far_plane);
  // frustum[12] = -2.0f * ox;
  // frustum[13] = -2.0f * oy;
  // frustum[14] = (far_plane + near_plane) / (near_plane - far_plane);
  // frustum[15] = 1.f;

  frustum[0][0] = 2.0f;
  frustum[1][1] = 2.0f;
  frustum[2][2] = 2.0f / (near_plane - far_plane);
  frustum[3][0] = -2 * ox;
  frustum[3][1] = -2 * oy;
  frustum[3][2] = (far_plane + near_plane) / (near_plane - far_plane);
  frustum[3][3] = 1.f;

  return frustum;
}

}
}
