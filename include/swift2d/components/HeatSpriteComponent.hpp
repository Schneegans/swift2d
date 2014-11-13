////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_HEAT_SPRITE_COMPONENT_HPP
#define SWIFT2D_HEAT_SPRITE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/components/DepthComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class HeatSpriteComponent;
typedef std::shared_ptr<HeatSpriteComponent>       HeatSpriteComponentPtr;
typedef std::shared_ptr<const HeatSpriteComponent> ConstHeatSpriteComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL HeatSpriteComponent : public TransformableComponent,
                                      public DepthComponent  {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public SerializedComponent {
    math::mat3 Transform;
    TexturePtr Texture;
    float      Opacity;
  };

  class Renderer : public ResourceRenderer<HeatSpriteComponent> {
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  TextureProperty Texture;
  Float           Opacity;

  // ----------------------------------------------------- contruction interface
  HeatSpriteComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static HeatSpriteComponentPtr create(Args&& ... a) {
    return std::make_shared<HeatSpriteComponent>(a...);
  }

  // creates a copy from this
  HeatSpriteComponentPtr create_copy() const {
    return std::make_shared<HeatSpriteComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "HeatSpriteComponent"; }

  virtual void update(double time);
  virtual void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_HEAT_SPRITE_COMPONENT_HPP
