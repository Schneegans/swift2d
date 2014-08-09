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
  PostProcessor(RenderContext const& ctx, int shading_quality, bool super_sampling);

  // ------------------------------------------------------------ public methods
  void process(ConstSerializedScenePtr const& scene, RenderContext const& ctx,
               oglplus::Texture const& final_buffer,
               oglplus::Texture const& g_buffer_light);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void generate_threshold_buffer(RenderContext const& ctx);

  int  shading_quality_;
  bool super_sampling_;

  Shader               post_fx_shader_;

  Shader               threshold_shader_;
  oglplus::Framebuffer threshold_fbo_;
  oglplus::Texture     threshold_buffer_;

  StreakEffect         streak_effect_;
  GhostEffect          ghost_effect_;
  HeatEffect           heat_effect_;

  Texture dirt_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_POST_PROCESSOR_HPP
