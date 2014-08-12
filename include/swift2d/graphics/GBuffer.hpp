////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GBUFFER_HPP
#define SWIFT2D_GBUFFER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RenderContext.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class GBuffer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  GBuffer(RenderContext const& ctx,
          bool final_buffer, bool diffuse_alpha = false,
          int sub_sample_level = 1);

  // ------------------------------------------------------------ public methods
  void bind_for_drawing(RenderContext const& ctx);
  void bind_final_buffer_for_drawing(RenderContext const& ctx);

  void bind_diffuse(int location);
  void bind_normal (int location);
  void bind_light  (int location);
  void bind_final  (int location);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  int sub_sample_level_;

  ogl::Framebuffer fbo_;
  ogl::Texture     diffuse_;
  ogl::Texture     normal_;
  ogl::Texture     light_;
  ogl::Texture     final_;
};

}

#endif // SWIFT2D_GBUFFER_HPP
