////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef FIBRGLASS_SPRITE_RESOURCE_HPP
#define FIBRGLASS_SPRITE_RESOURCE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/resources/GeometryResource.hpp>

#include <mutex>
#include <thread>
#include <vector>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Stores geometry data. A mesh can be loaded from an Assimp mesh and the     //
// draw onto a context.                                                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SpriteResource : public GeometryResource {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  SpriteResource();

  // Draws the SpriteResource to the given context.
  void draw(RenderContext const& context) const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context) const;

  mutable oglplus::Shader vs, fs;
  // Program
  mutable oglplus::Program prog;
  // A vertex array object for the rendered rectangle
  oglplus::VertexArray rectangle;
  // VBOs for the rectangle's vertices and colors
  oglplus::Buffer verts;
  oglplus::Buffer colors;
};

}

#endif // FIBRGLASS_SPRITE_RESOURCE_HPP
