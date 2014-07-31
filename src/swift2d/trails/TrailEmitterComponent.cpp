////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/trails/TrailEmitterComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TrailEmitterComponent::TrailEmitterComponent()
  : Life                  (10.f)
  , Density               (1.f)
  {}

////////////////////////////////////////////////////////////////////////////////

void TrailEmitterComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Life", Life);
  visitor.add_member("Density", Density);
}

////////////////////////////////////////////////////////////////////////////////

SerializedTrailEmitter TrailEmitterComponent::make_serialized_emitter() const {
  SerializedTrailEmitter result;

  result.Life = Life();
  result.Density = Density();
  result.WorldTransform = WorldTransform();
  result.Self = this;

  return result;
}

////////////////////////////////////////////////////////////////////////////////

}
