////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/LightComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LightComponent::LightComponent()
  : Depth(0.f) {}

////////////////////////////////////////////////////////////////////////////////

void LightComponent::draw(RenderContext const& ctx) {
  Material()->draw_quad(ctx, WorldTransform(), Depth());
}

////////////////////////////////////////////////////////////////////////////////

void LightComponent::serialize(SerializedScenePtr& scene) const {
  scene->lights.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void LightComponent::accept(SavableObjectVisitor& visitor) {
  DrawableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_object("Material", Material);
}

////////////////////////////////////////////////////////////////////////////////

}
