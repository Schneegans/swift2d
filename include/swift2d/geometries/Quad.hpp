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

#ifndef SWIFT2D_QUAD_HPP
#define SWIFT2D_QUAD_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/opengl.hpp>
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Stores geometry data of a quad. It can be drawn with OpenGL but any shader //
// must be made active before.                                                //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Quad: public Singleton<Quad> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Draws the Quad. Uses glDrawArrays internally
  void draw() const;

  // Draws the Quad multiple times. Uses
  // glDrawArraysInstanced internally
  void draw(unsigned instance_count) const;

  friend class Singleton<Quad>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Quad();
  ~Quad();

  void upload() const;

  mutable oglplus::VertexArray* rectangle_;
  mutable oglplus::Buffer*      verts_;
};

}

#endif // SWIFT2D_QUAD_HPP
