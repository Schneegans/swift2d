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
  : Life                   (10.f)
  , Density                (1.f)
  , prev_1_world_transform_()
  , prev_2_world_transform_()
  , prev_3_world_transform_()
  , first_frame_           (true)
  {}

////////////////////////////////////////////////////////////////////////////////

void TrailEmitterComponent::update(double time) {
  if (first_frame_) {
    prev_2_world_transform_ = WorldTransform();
    prev_1_world_transform_ = WorldTransform();
    first_frame_ = false;
  }

  prev_3_world_transform_ = prev_2_world_transform_;
  prev_2_world_transform_ = prev_1_world_transform_;
  prev_1_world_transform_ = WorldTransform();

  TransformableComponent::update(time);
}

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
  result.Prev1WorldTransform = prev_1_world_transform_;
  result.Prev2WorldTransform = prev_2_world_transform_;
  result.Prev3WorldTransform = prev_3_world_transform_;
  result.Self = this;

  return result;
}

////////////////////////////////////////////////////////////////////////////////

}
