////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_OPENGL_DEBUGGING_HPP
#define SWIFT2D_OPENGL_DEBUGGING_HPP

// includes  -------------------------------------------------------------------

#if defined( _WIN32 )
  #include <nvToolsExt.h>
  #define SWIFT_PUSH_GL_RANGE(name) nvtxRangePushA(name)
  #define SWIFT_POP_GL_RANGE()      nvtxRangePop()
#else // _WIN32
  #define SWIFT_PUSH_GL_RANGE(name)
  #define SWIFT_POP_GL_RANGE()
#endif // else _WIN32

#endif  // SWIFT2D_OPENGL_DEBUGGING_HPP
