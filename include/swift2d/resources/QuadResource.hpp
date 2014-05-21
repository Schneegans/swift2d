////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_QUAD_RESOURCE_HPP
#define SWIFT2D_QUAD_RESOURCE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Stores geometry data. A mesh can be loaded from an Assimp mesh and the     //
// draw onto a context.                                                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class QuadResource: public Singleton<QuadResource> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Draws the QuadResource to the given context.
  void draw(RenderContext const& context) const;

  friend class Singleton<QuadResource>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  QuadResource();
  ~QuadResource();

  void upload_to(RenderContext const& context) const;

  mutable oglplus::VertexArray* rectangle_;
  mutable oglplus::Buffer*      verts_;
};

}

#endif // SWIFT2D_QUAD_RESOURCE_HPP
