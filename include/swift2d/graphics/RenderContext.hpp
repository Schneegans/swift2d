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

#ifndef SWIFT2D_RENDER_CONTEXT_HPP
#define SWIFT2D_RENDER_CONTEXT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/math.hpp>

namespace swift {

class Pipeline;

////////////////////////////////////////////////////////////////////////////////
// Stores all relevant information on a OpenGL context.                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
struct RenderContext {

  float time;

  math::vec2i window_size;
  math::vec2i g_buffer_size;
  math::mat3  projection_matrix;
  float       projection_parallax;
  bool        ready;

  bool sub_sampling;
  bool dynamic_lighting;
  bool light_sub_sampling;
  bool lens_flares;
  bool heat_effect;

  mutable int upload_budget;
  mutable int upload_remaining;

  Pipeline* pipeline;
};

}

#endif // SWIFT2D_RENDER_CONTEXT_HPP
