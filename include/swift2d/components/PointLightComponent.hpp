////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_POINT_LIGHT_COMPONENT_HPP
#define SWIFT2D_POINT_LIGHT_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/DrawableComponent.hpp>

#include <swift2d/resources/LightResource.hpp>
#include <swift2d/resources/TextureResource.hpp>

#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class PointLightComponent;
typedef std::shared_ptr<PointLightComponent>       PointLightComponentPtr;
typedef std::shared_ptr<const PointLightComponent> ConstPointLightComponentPtr;

// -----------------------------------------------------------------------------
class PointLightComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float                   Depth;
  LightResourceProperty   Sprite;
  TextureResourceProperty Tex;

  // ----------------------------------------------------- contruction interface
  PointLightComponent() : Sprite(nullptr), Tex(nullptr) {}

  // Creates a new component and returns a shared pointer.
  static PointLightComponentPtr create() {
    return std::make_shared<PointLightComponent>();
  }

  PointLightComponentPtr create_copy() const {
    return std::make_shared<PointLightComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  void draw(RenderContext const& ctx) {
    Tex()->bind(ctx, 0);
    Sprite()->draw(ctx, WorldTransform.get());
  }

  void serialize(SerializedScenePtr& scene) const {
    scene->lights.insert(std::make_pair(Depth.get(), create_copy()));
  }


 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

};

}

#endif  // SWIFT2D_POINT_LIGHT_COMPONENT_HPP
