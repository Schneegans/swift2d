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

#ifndef SWIFT2D_GLOW_EFFECT_HPP
#define SWIFT2D_GLOW_EFFECT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/scene/SerializedScene.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class GlowEffect {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  GlowEffect(RenderContext const& ctx);

  void process(RenderContext const& ctx);
  int bind_buffers(int start, RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  Shader hdr_shader_;
  Shader blur_shader_;
  Shader downsample_shader_;

  oglplus::Lazy<oglplus::Uniform<math::vec2>> blur_size_;
  oglplus::Lazy<oglplus::Uniform<int>>        input_tex_;
  oglplus::Lazy<oglplus::Uniform<int>>        blur_tex_;
  oglplus::Lazy<oglplus::Uniform<int>>        hdr_tex_;
  oglplus::Lazy<oglplus::Uniform<int>>        emit_tex_;

  struct BlurData {
    oglplus::Framebuffer fbo_;
    oglplus::Texture     buffer_1_;
    oglplus::Texture     buffer_2_;
  };
  std::vector<BlurData>  blur_data_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_GLOW_EFFECT_HPP
