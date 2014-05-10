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
#include <GL/glew.h>
#include <oglplus/all.hpp>
#include <oglplus/opt/smart_enums.hpp>
#include <oglplus/bound/texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Stores all relevant information on a OpenGL context.                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
struct RenderContext {

  oglplus::Context gl;

  int width;
  int height;
};

}

#endif // SWIFT2D_RENDERCONTEXT_HPP
