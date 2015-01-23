////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SUB_SAMPLE_BUFFER_HPP
#define SWIFT2D_SUB_SAMPLE_BUFFER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/opengl.hpp>
#include <swift2d/graphics/RenderContext.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class SubSampleBuffer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  SubSampleBuffer(RenderContext const& ctx, int sub_sample_level = 1);

  // ------------------------------------------------------------ public methods
  void bind_for_drawing(RenderContext const& ctx);

  void bind_diffuse(int location);
  void bind_normal (int location);
  void bind_light  (int location);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  int sub_sample_level_;

  ogl::Framebuffer fbo_;
  ogl::Texture     diffuse_;
  ogl::Texture     normal_;
  ogl::Texture     light_;
};

}

#endif // SWIFT2D_SUB_SAMPLE_BUFFER_HPP
