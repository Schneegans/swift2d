////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_RENDER_CONTEXT_HPP
#define SWIFT2D_RENDER_CONTEXT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/opengl.hpp>
#include <swift2d/math.hpp>

namespace swift {

class Pipeline;

////////////////////////////////////////////////////////////////////////////////
// Stores all relevant information on a OpenGL context.                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
struct RenderContext {

  oglplus::Context gl;

  math::vec2i window_size;
  math::vec2i g_buffer_size;
  math::mat3  projection_matrix;
  float       projection_parallax;
  bool        ready;

  int  shading_quality;
  bool sub_sampling;
  bool light_sub_sampling;

  mutable int upload_budget;
  mutable int upload_remaining;

  Pipeline* pipeline;
};

}

#endif // SWIFT2D_RENDER_CONTEXT_HPP
