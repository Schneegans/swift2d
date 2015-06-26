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

#ifndef SWIFT2D_GBUFFER_HPP
#define SWIFT2D_GBUFFER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/opengl.hpp>
#include <swift2d/graphics/RenderContext.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class GBuffer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  GBuffer(RenderContext const& ctx);

  // ------------------------------------------------------------ public methods
  void bind_for_drawing(RenderContext const& ctx);

  int bind_diffuse(int location);
  int bind_normal (int location);
  int bind_light  (int location);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ogl::Framebuffer fbo_;
  ogl::Texture     diffuse_;
  ogl::Texture     normal_;
  ogl::Texture     light_;
};

}

#endif // SWIFT2D_GBUFFER_HPP
