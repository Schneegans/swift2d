////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef FIBRGLASS_SCREEN_QUAD_RESOURCE_HPP
#define FIBRGLASS_SCREEN_QUAD_RESOURCE_HPP

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
class ScreenQuadResource: public Singleton<ScreenQuadResource> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Draws the ScreenQuadResource to the given context.
  void draw(RenderContext const& context) const;

  friend class Singleton<ScreenQuadResource>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  ScreenQuadResource();
  ~ScreenQuadResource();

  void upload_to(RenderContext const& context) const;

  mutable oglplus::VertexArray* rectangle_;
  mutable oglplus::Buffer*      verts_;
};

}

#endif // FIBRGLASS_SCREEN_QUAD_RESOURCE_HPP
