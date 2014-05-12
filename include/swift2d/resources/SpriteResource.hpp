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

#include <mutex>
#include <thread>
#include <vector>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Stores geometry data. A mesh can be loaded from an Assimp mesh and the     //
// draw onto a context.                                                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SpriteResource {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  SpriteResource();
  ~SpriteResource();

  // Draws the SpriteResource to the given context.
  void draw(RenderContext const& context, math::mat3 const& transform, bool with_normals) const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context) const;

  mutable oglplus::Shader  *vs, *fs;
  mutable oglplus::Program *prog;

  mutable oglplus::VertexArray *rectangle;
  mutable oglplus::Buffer      *verts;
};

}

#endif // FIBRGLASS_SPRITE_RESOURCE_HPP
