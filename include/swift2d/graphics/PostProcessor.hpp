////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_POST_PROCESSOR_HPP
#define SWIFT2D_POST_PROCESSOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/graphics/StreakEffect.hpp>
#include <swift2d/graphics/GhostEffect.hpp>
#include <swift2d/graphics/HeatEffect.hpp>
#include <swift2d/graphics/GBuffer.hpp>
#include <swift2d/textures/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class PostProcessor {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  PostProcessor(RenderContext const& ctx);

  // ------------------------------------------------------------ public methods
  void process(ConstSerializedScenePtr const& scene, RenderContext const& ctx,
               GBuffer* g_buffer);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void generate_threshold_buffer(RenderContext const& ctx);

  Shader post_fx_shader_;
  Shader threshold_shader_;

  oglplus::Lazy<oglplus::Uniform<int>> g_buffer_shaded_;
  oglplus::Lazy<oglplus::Uniform<int>> glow_buffer_1_;
  oglplus::Lazy<oglplus::Uniform<int>> glow_buffer_2_;
  oglplus::Lazy<oglplus::Uniform<int>> glow_buffer_3_;
  oglplus::Lazy<oglplus::Uniform<int>> glow_buffer_4_;
  oglplus::Lazy<oglplus::Uniform<int>> glow_buffer_5_;
  oglplus::Lazy<oglplus::Uniform<int>> glow_buffer_6_;
  oglplus::Lazy<oglplus::Uniform<int>> glow_buffer_7_;
  oglplus::Lazy<oglplus::Uniform<int>> glow_buffer_8_;
  oglplus::Lazy<oglplus::Uniform<int>> heat_buffer_;
  oglplus::Lazy<oglplus::Uniform<int>> dirt_tex_;
  oglplus::Lazy<oglplus::Uniform<float>> dirt_opacity_;
  oglplus::Lazy<oglplus::Uniform<int>> use_heat_;
  oglplus::Lazy<oglplus::Uniform<float>> gamma_;
  oglplus::Lazy<oglplus::Uniform<math::vec4>> vignette_color_;
  oglplus::Lazy<oglplus::Uniform<float>> vignette_softness_;
  oglplus::Lazy<oglplus::Uniform<float>> vignette_coverage_;
  oglplus::Lazy<oglplus::Uniform<math::vec2i>> screen_size_;
  oglplus::Lazy<oglplus::Uniform<int>> g_buffer_diffuse_;
  oglplus::Lazy<oglplus::Uniform<int>> g_buffer_light_;
  oglplus::Lazy<oglplus::Uniform<int>> use_color_grading_;
  oglplus::Lazy<oglplus::Uniform<int>> color_grading_tex_;

  oglplus::Framebuffer threshold_fbo_;
  oglplus::Texture     threshold_buffer_;

  StreakEffect         streak_effect_;
  GhostEffect          ghost_effect_;
  HeatEffect           heat_effect_;

  Texture   dirt_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_POST_PROCESSOR_HPP
