////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_FULLSCREEN_SPRITE_COMPONENT_HPP
#define SWIFT2D_FULLSCREEN_SPRITE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/components/DepthComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/materials/Material.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class FullscreenSpriteComponent;
typedef std::shared_ptr<FullscreenSpriteComponent>       FullscreenSpriteComponentPtr;
typedef std::shared_ptr<const FullscreenSpriteComponent> ConstFullscreenSpriteComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL FullscreenSpriteComponent : public Component,
                                            public DepthComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public SerializedComponent {
    MaterialBasePtr Material;
  };

  class Renderer : public ResourceRenderer<FullscreenSpriteComponent> {
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  MaterialProperty      Material;
  MaterialBaseProperty  CustomMaterial; // used, if the above is not set

  // ----------------------------------------------------- contruction interface
  FullscreenSpriteComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static FullscreenSpriteComponentPtr create(Args&& ... a) {
    return std::make_shared<FullscreenSpriteComponent>(a...);
  }

  // creates a copy from this
  FullscreenSpriteComponentPtr create_copy() const {
    return std::make_shared<FullscreenSpriteComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "FullscreenSpriteComponent"; }

  virtual void update(double time);
  virtual void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_FULLSCREEN_SPRITE_COMPONENT_HPP
