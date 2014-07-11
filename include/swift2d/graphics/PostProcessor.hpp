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
  PostProcessor(RenderContext const& ctx, int shading_quality);

  // ------------------------------------------------------------ public methods
  void draw_heat_objects(ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void process(RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:


  void generate_threshold_buffer(RenderContext const& ctx);
  void generate_streak_buffers(RenderContext const& ctx);

  int shading_quality_;

  Shader               post_fx_shader_;

  Shader               threshold_shader_;
  oglplus::Framebuffer threshold_fbo_;
  oglplus::Texture     threshold_buffer_;

  Shader               streak_shader_;
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

  // Shader               glow_shader_;
  // oglplus::Framebuffer glow_fbo_;
  // oglplus::Texture     glow_ping_;
  // oglplus::Texture     glow_pong_;

  oglplus::Framebuffer heat_fbo_;
  oglplus::Texture     heat_buffer_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_POST_PROCESSOR_HPP
