////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_FRUSTUM_HPP
#define SWIFT2D_FRUSTUM_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/math/math.hpp>

#include <vector>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Frustum {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface

  static Frustum perspective(
    glm::mat4 const& camera_transform,
    glm::mat4 const& screen_transform,
    float clip_near,
    float clip_far
  );

  static Frustum orthographic(
    glm::mat4 const& camera_transform,
    glm::mat4 const& screen_transform,
    float clip_near,
    float clip_far
  );

  Frustum();

  // ------------------------------------------------------------------- getters

  std::vector<glm::vec3> get_corners() const;

  inline glm::vec3 get_camera_position() const {
    return glm::vec3(
      camera_transform_[3][0],
      camera_transform_[3][1],
      camera_transform_[3][2]
    );
  }

  inline glm::mat4 const& get_camera_transform() const {
    return camera_transform_;
  }

  inline glm::mat4 const& get_screen_transform() const {
    return screen_transform_;
  }

  inline glm::mat4 const& get_projection() const {
    return projection_;
  }

  inline glm::mat4 const& get_view() const {
    return view_;
  }

  inline float get_clip_near() const {
    return clip_near_;
  }

  inline float get_clip_far() const {
    return clip_far_;
  }


 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  static void init_frustum_members(
    glm::mat4 const& camera_transform,
    glm::mat4 const& screen_transform,
    Frustum& frustum
  );

  glm::mat4               camera_transform_;
  glm::mat4               screen_transform_;
  glm::mat4               projection_;
  glm::mat4               view_;
  std::vector<glm::vec4>  planes_;
  float                   clip_near_;
  float                   clip_far_;
};

}

#endif // SWIFT2D_FRUSTUM_HPP
