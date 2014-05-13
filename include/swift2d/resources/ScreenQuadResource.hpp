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
#include <swift2d/events/properties.hpp>

#include <mutex>
#include <thread>
#include <vector>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Stores geometry data. A mesh can be loaded from an Assimp mesh and the     //
// draw onto a context.                                                       //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ScreenQuadResource;
typedef std::shared_ptr<ScreenQuadResource>       ScreenQuadResourcePtr;
typedef std::shared_ptr<const ScreenQuadResource> ConstScreenQuadResourcePtr;
typedef Property<ScreenQuadResourcePtr>           ScreenQuadResourceProperty;

// -----------------------------------------------------------------------------
class ScreenQuadResource {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  template <typename... Args>
  static ScreenQuadResourcePtr create(Args&& ... a) {
    return std::make_shared<ScreenQuadResource>(a...);
  }

  ScreenQuadResource();
  ~ScreenQuadResource();

  // Draws the ScreenQuadResource to the given context.
  void draw(RenderContext const& context) const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context) const;

  mutable oglplus::VertexArray* rectangle_;
  mutable oglplus::Buffer*      verts_;
};

}

#endif // FIBRGLASS_SCREEN_QUAD_RESOURCE_HPP
