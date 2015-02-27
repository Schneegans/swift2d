////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/components/DepthComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DepthComponent::DepthComponent()
  : Depth(0)
  , WorldDepth(0) {}

////////////////////////////////////////////////////////////////////////////////

void DepthComponent::update(double time, SceneObject* user) {
  if (user) {
    WorldDepth = user->WorldDepth.get() + Depth.get();
  } else {
    WorldDepth = Depth.get();
  }
}

////////////////////////////////////////////////////////////////////////////////

void DepthComponent::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("Depth", Depth);
}

////////////////////////////////////////////////////////////////////////////////

}
