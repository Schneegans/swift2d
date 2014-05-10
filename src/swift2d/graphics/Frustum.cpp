////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Frustum.hpp>

#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Frustum::Frustum()
  : camera_transform_(glm::mat4(1.f)),
    screen_transform_(glm::mat4(1.f)),
    projection_(glm::mat4(1.f)),
    view_(glm::mat4(1.f)),
    planes_(6),
    clip_near_(0),
    clip_far_(0) {}

////////////////////////////////////////////////////////////////////////////////

Frustum Frustum::perspective(glm::mat4 const& camera_transform,
                             glm::mat4 const& screen_transform,
                             float clip_near,
                             float clip_far) {

  auto projection = math::make_perspective(
             camera_transform[3], screen_transform, clip_near, clip_far);

  Frustum result;

  result.projection_ = projection;
  result.clip_near_ = clip_near;
  result.clip_far_ = clip_far;

  init_frustum_members(camera_transform, screen_transform, result);

  return result;
}

////////////////////////////////////////////////////////////////////////////////

Frustum Frustum::orthographic(glm::mat4 const& camera_transform,
                              glm::mat4 const& screen_transform,
                              float clip_near,
                              float clip_far) {

  auto projection = math::make_orthographic(
             camera_transform[3], screen_transform, clip_near, clip_far);

  Frustum result;

  result.projection_ = projection;
  result.clip_near_ = clip_near;
  result.clip_far_ = clip_far;

  init_frustum_members(camera_transform, screen_transform, result);

  return result;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<glm::vec3> Frustum::get_corners() const {
  std::vector<glm::vec4> tmp(8);
  std::vector<glm::vec3> result(8);

  auto inverse_transform(glm::inverse(projection_ * view_));

  tmp[0] = inverse_transform * glm::vec4(-1, -1, -1, 1);
  tmp[1] = inverse_transform * glm::vec4(-1, -1,  1, 1);
  tmp[2] = inverse_transform * glm::vec4(-1,  1, -1, 1);
  tmp[3] = inverse_transform * glm::vec4(-1,  1,  1, 1);
  tmp[4] = inverse_transform * glm::vec4( 1, -1, -1, 1);
  tmp[5] = inverse_transform * glm::vec4( 1, -1,  1, 1);
  tmp[6] = inverse_transform * glm::vec4( 1,  1, -1, 1);
  tmp[7] = inverse_transform * glm::vec4( 1,  1,  1, 1);

  for (int i(0); i<8; ++i) {
    result[i] = glm::swizzle<glm::X, glm::Y, glm::Z>(tmp[i]/tmp[i][3]);
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

void Frustum::init_frustum_members(glm::mat4 const& camera_transform,
                         glm::mat4 const& screen_transform,
                         Frustum& frustum) {

  glm::mat4 view_transform(screen_transform);
  view_transform[3][0] = 0.f;
  view_transform[3][1] = 0.f;
  view_transform[3][2] = 0.f;
  view_transform[3][3] = 1.f;

  frustum.camera_transform_ = camera_transform;
  frustum.screen_transform_ = screen_transform;

  view_transform =
      math::make_translate(frustum.get_camera_position()) * view_transform;

  frustum.view_ = glm::inverse(view_transform);

  auto projection_view(frustum.projection_ * frustum.view_);

  //store normals

  // // left plane
  // frustum.planes_[0] = glm::vec4(projection_view[3]  + projection_view[0],
  //                                projection_view[7]  + projection_view[4],
  //                                projection_view[11] + projection_view[8],
  //                                projection_view[15] + projection_view[12]);

  // //right plane
  // frustum.planes_[1] = glm::vec4(projection_view[3]  - projection_view[0],
  //                                projection_view[7]  - projection_view[4],
  //                                projection_view[11] - projection_view[8],
  //                                projection_view[15] - projection_view[12]);

  // //bottom plane
  // frustum.planes_[2] = glm::vec4(projection_view[3]  + projection_view[1],
  //                                projection_view[7]  + projection_view[5],
  //                                projection_view[11] + projection_view[9],
  //                                projection_view[15] + projection_view[13]);

  // //top plane
  // frustum.planes_[3] = glm::vec4(projection_view[3]  - projection_view[1],
  //                                projection_view[7]  - projection_view[5],
  //                                projection_view[11] - projection_view[9],
  //                                projection_view[15] - projection_view[13]);

  // //near plane
  // frustum.planes_[4] = glm::vec4(projection_view[3]  + projection_view[2],
  //                                projection_view[7]  + projection_view[6],
  //                                projection_view[11] + projection_view[10],
  //                                projection_view[15] + projection_view[14]);

  // //far plane
  // frustum.planes_[5] = glm::vec4(projection_view[3]  - projection_view[2],
  //                                projection_view[7]  - projection_view[6],
  //                                projection_view[11] - projection_view[10],
  //                                projection_view[15] - projection_view[14]);

  //left plane
  frustum.planes_[0] = glm::vec4(projection_view[0][3] + projection_view[0][0],
                                 projection_view[1][3] + projection_view[1][0],
                                 projection_view[2][3] + projection_view[2][0],
                                 projection_view[3][3] + projection_view[3][0]);

  //right plane
  frustum.planes_[1] = glm::vec4(projection_view[0][3] - projection_view[0][0],
                                 projection_view[1][3] - projection_view[1][0],
                                 projection_view[2][3] - projection_view[2][0],
                                 projection_view[3][3] - projection_view[3][0]);

  //bottom plane
  frustum.planes_[2] = glm::vec4(projection_view[0][3] + projection_view[0][1],
                                 projection_view[1][3] + projection_view[1][1],
                                 projection_view[2][3] + projection_view[2][1],
                                 projection_view[3][3] + projection_view[3][1]);

  //top plane
  frustum.planes_[3] = glm::vec4(projection_view[0][3] - projection_view[0][1],
                                 projection_view[1][3] - projection_view[1][1],
                                 projection_view[2][3] - projection_view[2][1],
                                 projection_view[3][3] - projection_view[3][1]);

  //near plane
  frustum.planes_[4] = glm::vec4(projection_view[0][3] + projection_view[0][2],
                                 projection_view[1][3] + projection_view[1][2],
                                 projection_view[2][3] + projection_view[2][2],
                                 projection_view[3][3] + projection_view[3][2]);

  //far plane
  frustum.planes_[5] = glm::vec4(projection_view[0][3] - projection_view[0][2],
                                 projection_view[1][3] - projection_view[1][2],
                                 projection_view[2][3] - projection_view[2][2],
                                 projection_view[3][3] - projection_view[3][2]);
}

////////////////////////////////////////////////////////////////////////////////

}
