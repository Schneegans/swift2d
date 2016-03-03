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

#ifndef SWIFT2D_STREAK_EFFECT_HPP
#define SWIFT2D_STREAK_EFFECT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/scene/SerializedScene.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class StreakEffect {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  StreakEffect(RenderContext const& ctx);

  void process(RenderContext const& ctx, oglplus::Texture const& threshold_buffer_);
  int bind_buffers(int start, RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  Shader streak_shader_;

  oglplus::Lazy<oglplus::Uniform<math::vec2>> step_;
  oglplus::Lazy<oglplus::Uniform<int>>        input_tex_;
  oglplus::Lazy<oglplus::Uniform<math::vec3>> colors_;

  oglplus::Framebuffer streak_fbo_;
  oglplus::Texture     streak_buffer_tmp_;
  oglplus::Texture     streak_buffer_1_;
  oglplus::Texture     streak_buffer_2_;

  std::vector<math::vec3> streak_colors_1_;
  std::vector<math::vec3> streak_colors_2_;
  std::vector<math::vec3> streak_colors_3_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_STREAK_EFFECT_HPP
