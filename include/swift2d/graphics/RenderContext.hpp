////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_RENDERCONTEXT_HPP
#define SWIFT2D_RENDERCONTEXT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/opengl.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Stores all relevant information on a OpenGL context.                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
struct RenderContext {

  oglplus::Context gl;
  math::vec2i size;
};

}

#endif // SWIFT2D_RENDERCONTEXT_HPP
