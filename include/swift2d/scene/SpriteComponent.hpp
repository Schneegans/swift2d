////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_COMPONENT_HPP
#define SWIFT2D_SPRITE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/DrawableComponent.hpp>

#include <swift2d/resources/SpriteResource.hpp>
#include <swift2d/resources/TextureResource.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SpriteComponent;
typedef std::shared_ptr<SpriteComponent>       SpriteComponentPtr;
typedef std::shared_ptr<const SpriteComponent> ConstSpriteComponentPtr;

// -----------------------------------------------------------------------------
class SpriteComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  String pTexture;
  Float  pDepth;

  // ----------------------------------------------------- contruction interface
  SpriteComponent() : sprite_(nullptr), tex_(nullptr) {}

  // Creates a new component and returns a shared pointer.
  static SpriteComponentPtr create() {
    return std::make_shared<SpriteComponent>();
  }

  SpriteComponentPtr create_copy() const {
    return std::make_shared<SpriteComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  void draw(RenderContext const& ctx) {

    if (!sprite_ || !tex_) {
      sprite_ = new SpriteResource();
      tex_ = new TextureResource(pTexture.get());
    }

    tex_->bind(ctx, 0);
    sprite_->draw(ctx, pWorldTransform.get());
  }

  void serialize(SerializedScenePtr& scene) const {
    scene->objects[pDepth.get()] = create_copy();
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  SpriteResource* sprite_;
  TextureResource* tex_;

};

}

#endif  // SWIFT2D_SPRITE_COMPONENT_HPP
