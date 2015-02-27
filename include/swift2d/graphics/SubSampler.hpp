////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SUB_SAMPLER_HPP
#define SWIFT2D_SUB_SAMPLER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/SubSampleBuffer.hpp>
#include <swift2d/materials/Shader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SubSampler {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  SubSampler(RenderContext const& ctx, int level);

  void bind(RenderContext const& context);
  void draw(RenderContext const& context);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Shader shader_;
  int    level_;

  oglplus::Reference<ogl::Framebuffer> original_framebuffer_;
  SubSampleBuffer buffer_;
};

}

#endif // SWIFT2D_SUB_SAMPLER_HPP
