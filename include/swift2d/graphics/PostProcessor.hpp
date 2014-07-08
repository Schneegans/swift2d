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
  void process(RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Shader               post_fx_shader_;
  Shader               glow_threshold_shader_;
  Shader               glow_shader_;

  oglplus::Framebuffer glow_fbo_;
  oglplus::Texture     glow_ping_;
  oglplus::Texture     glow_pong_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_POST_PROCESSOR_HPP
