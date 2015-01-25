////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GRAVITY_MAP_HPP
#define SWIFT2D_GRAVITY_MAP_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/scene/SerializedScene.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class GravityMap {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  GravityMap(RenderContext const& ctx);

  // ------------------------------------------------------------ public methods
  void process(ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void bind(RenderContext const& ctx, int location);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  oglplus::Framebuffer  gravity_fbo_;
  oglplus::Texture      gravity_map_;
  Shader                gravity_shader_;

  oglplus::Lazy<oglplus::Uniform<math::vec3>>  gravity_sources_;
  oglplus::Lazy<oglplus::Uniform<math::vec2i>> screen_size_;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>  world_gravity_;
  oglplus::Lazy<oglplus::Uniform<int>>         gravity_source_count_;
};

}

#endif // SWIFT2D_GRAVITY_MAP_HPP
