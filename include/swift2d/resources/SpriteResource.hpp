////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_RESOURCE_HPP
#define SWIFT2D_SPRITE_RESOURCE_HPP

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
class SpriteResource: public Singleton<SpriteResource> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Draws the SpriteResource to the given context.
  void draw(RenderContext const& context, math::mat3 const& transform, bool with_normals, float emit) const;

  friend class Singleton<SpriteResource>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  SpriteResource();
  ~SpriteResource();

  void upload_to(RenderContext const& context) const;

  mutable oglplus::Shader  *vs, *fs;
  mutable oglplus::Program *prog;

  mutable oglplus::VertexArray *rectangle;
  mutable oglplus::Buffer      *verts;
};

}

#endif // SWIFT2D_SPRITE_RESOURCE_HPP
