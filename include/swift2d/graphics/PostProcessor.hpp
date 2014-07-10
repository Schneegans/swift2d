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
  int shading_quality_;

  Shader               post_fx_shader_;
  Shader               glow_threshold_shader_;
  Shader               glow_shader_;

  oglplus::Framebuffer glow_fbo_;
  oglplus::Texture     glow_ping_;
  oglplus::Texture     glow_pong_;

  oglplus::Framebuffer heat_fbo_;
  oglplus::Texture     heat_buffer_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_POST_PROCESSOR_HPP
