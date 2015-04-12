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

#ifndef SWIFT2D_LENS_FLARE_EFFECT_HPP
#define SWIFT2D_LENS_FLARE_EFFECT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/graphics/StreakEffect.hpp>
#include <swift2d/graphics/GhostEffect.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class LensFlareEffect {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  LensFlareEffect(RenderContext const& ctx);

  void process(RenderContext const& ctx);
  void bind_buffer(int location, RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void generate_threshold_buffer(RenderContext const& ctx);

  Shader mix_shader_;
  Shader threshold_shader_;

  oglplus::Lazy<oglplus::Uniform<int>>         inputs_;
  oglplus::Lazy<oglplus::Uniform<math::vec2i>> screen_size_;
  oglplus::Lazy<oglplus::Uniform<int>>         g_buffer_diffuse_;
  oglplus::Lazy<oglplus::Uniform<int>>         g_buffer_light_;

  GhostEffect  ghosts_;
  StreakEffect streaks_;

  oglplus::Framebuffer fbo_;
  oglplus::Texture     buffer_;
};

  // -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_LENS_FLARE_EFFECT_HPP
