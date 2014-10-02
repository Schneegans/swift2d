////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
  oglplus::Texture     streak_buffer_3_;
  oglplus::Texture     streak_buffer_4_;
  oglplus::Texture     streak_buffer_5_;
  oglplus::Texture     streak_buffer_6_;

  std::vector<math::vec3> streak_colors_1_;
  std::vector<math::vec3> streak_colors_2_;
  std::vector<math::vec3> streak_colors_3_;
  std::vector<math::vec3> streak_colors_4_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_STREAK_EFFECT_HPP
