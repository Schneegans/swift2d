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

#ifndef SWIFT2D_POST_PROCESSOR_HPP
#define SWIFT2D_POST_PROCESSOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/graphics/LensFlareEffect.hpp>
#include <swift2d/graphics/HeatEffect.hpp>
#include <swift2d/graphics/GBuffer.hpp>
#include <swift2d/graphics/LBuffer.hpp>
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
               GBuffer* g_buffer, LBuffer* l_buffer);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Shader post_fx_shader_;

  oglplus::Lazy<oglplus::Uniform<int>> g_buffer_diffuse_;
  oglplus::Lazy<oglplus::Uniform<int>> g_buffer_normal_;
  oglplus::Lazy<oglplus::Uniform<math::vec2i>> screen_size_;
  oglplus::Lazy<oglplus::Uniform<int>> l_buffer_;
  oglplus::Lazy<oglplus::Uniform<int>> glow_buffer_;
  oglplus::Lazy<oglplus::Uniform<int>> heat_buffer_;
  oglplus::Lazy<oglplus::Uniform<int>> dirt_tex_;
  oglplus::Lazy<oglplus::Uniform<float>> dirt_opacity_;
  oglplus::Lazy<oglplus::Uniform<float>> glow_opacity_;
  oglplus::Lazy<oglplus::Uniform<int>> use_heat_;
  oglplus::Lazy<oglplus::Uniform<float>> gamma_;
  oglplus::Lazy<oglplus::Uniform<math::vec4>> vignette_color_;
  oglplus::Lazy<oglplus::Uniform<float>> vignette_softness_;
  oglplus::Lazy<oglplus::Uniform<float>> vignette_coverage_;
  oglplus::Lazy<oglplus::Uniform<int>> use_color_grading_;
  oglplus::Lazy<oglplus::Uniform<int>> color_grading_tex_;
  oglplus::Lazy<oglplus::Uniform<float>> color_grading_intensity_;

  HeatEffect      heat_effect_;
  LensFlareEffect lens_flare_effect_;
  Texture         dirt_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_POST_PROCESSOR_HPP
