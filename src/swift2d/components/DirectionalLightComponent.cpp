////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/DirectionalLightComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DirectionalLightComponent::DirectionalLightComponent()
  : Depth(0.f)
  , Direction(math::vec3(0, 0, -1))
  , Color(swift::Color(1.f, 1.f, 1.f)) {}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightComponent::draw(RenderContext const& ctx) {
    // performed by the compositor!
}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightComponent::serialize(SerializedScenePtr& scene) const {
  scene->sun_lights.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightComponent::accept(SavableObjectVisitor& visitor) {
  DrawableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_member("Direction", Direction);
  visitor.add_member("Color", Color);
}

////////////////////////////////////////////////////////////////////////////////

}
