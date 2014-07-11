////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_COMPOSITOR_HPP
#define SWIFT2D_COMPOSITOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/PostProcessor.hpp>
#include <swift2d/materials/Shader.hpp>
#include <swift2d/properties.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class Compositor {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  Compositor(RenderContext const& ctx, int shading_quality);
  ~Compositor();

  // ------------------------------------------------------------ public methods
  void draw_objects       (ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void draw_lights        (ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void post_process       (ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void draw_gui           (ConstSerializedScenePtr const& scene, RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  int shading_quality_;

  oglplus::Framebuffer  fbo_;
  oglplus::Texture      g_buffer_diffuse_;
  oglplus::Texture      g_buffer_normal_;
  oglplus::Texture      g_buffer_light_;
  oglplus::Texture      final_buffer_;

  Shader*        shader_;
  PostProcessor* post_processor_;
};

}

#endif // SWIFT2D_COMPOSITOR_HPP
