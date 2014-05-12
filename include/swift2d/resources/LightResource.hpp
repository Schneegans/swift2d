////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef FIBRGLASS_LIGHT_RESOURCE_HPP
#define FIBRGLASS_LIGHT_RESOURCE_HPP

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
class LightResource {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  LightResource();
  ~LightResource();

  // Draws the LightResource to the given context.
  void draw(RenderContext const& context, math::mat3 const& transform) const;

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

#endif // FIBRGLASS_LIGHT_RESOURCE_HPP