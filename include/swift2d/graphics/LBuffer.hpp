////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LBUFFER_HPP
#define SWIFT2D_LBUFFER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/opengl.hpp>
#include <swift2d/graphics/RenderContext.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class LBuffer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  LBuffer(RenderContext const& ctx);

  // ------------------------------------------------------------ public methods
  void bind_for_drawing(RenderContext const& ctx);
  void bind(int location);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ogl::Framebuffer fbo_;
  ogl::Texture     buffer_;
};

}

#endif // SWIFT2D_LBUFFER_HPP
