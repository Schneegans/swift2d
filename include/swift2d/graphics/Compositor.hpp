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
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/materials/Shader.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/properties.hpp>

namespace swift {

// shared pointer type definition ----------------------------------------------
class Compositor;
typedef std::shared_ptr<Compositor>       CompositorPtr;
typedef std::shared_ptr<const Compositor> ConstCompositorPtr;
typedef Property<CompositorPtr>           CompositorProperty;

// -----------------------------------------------------------------------------
class Compositor {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Bool EnableDynamicLighting;

  // ---------------------------------------------------- construction interface
  template <typename... Args>
  static CompositorPtr create(Args&& ... a) {
    return std::make_shared<Compositor>(a...);
  }

  Compositor();
  ~Compositor();

  // ------------------------------------------------------------ public methods

  void init(RenderContext const& ctx);

  void draw_objects(ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void draw_lights(ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void composite(ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void draw_gui(RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context) const;
  void clean_up();

  mutable Shader* shader_;
  oglplus::Framebuffer* fbo_;
  oglplus::Texture* offscreen_color_;
  oglplus::Texture* offscreen_normal_;
  oglplus::Texture* offscreen_light_;
  oglplus::Texture* offscreen_aux_;
};

}

#endif // SWIFT2D_COMPOSITOR_HPP
