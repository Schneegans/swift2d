////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
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
// Stores geometry data. A mesh can be loaded from an Assimp mesh and the     //
// draw onto a context.                                                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Quad: public Singleton<Quad> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Draws the Quad to the given context.
  void draw(RenderContext const& context) const;
  void draw(RenderContext const& context, unsigned instance_count) const;

  friend class Singleton<Quad>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Quad();
  ~Quad();

  void upload_to(RenderContext const& context) const;

  mutable oglplus::VertexArray* rectangle_;
  mutable oglplus::Buffer*      verts_;
};

}

#endif // SWIFT2D_QUAD_HPP
