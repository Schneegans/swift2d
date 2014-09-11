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
  , position_()
  , last_position_()
  , prev_1_position_()
  , prev_2_position_()
  , prev_3_position_()
  , first_frame_           (true)
  , spawn_new_point_       (false)
  {}

////////////////////////////////////////////////////////////////////////////////

void TrailEmitterComponent::update(double time) {
  if (first_frame_) {
    last_position_ = (WorldTransform() * math::vec3(0.0, 0.0, 1)).xy();
    prev_3_position_ = (WorldTransform() * math::vec3(0.0, 0.0, 1)).xy();
    prev_2_position_ = (WorldTransform() * math::vec3(0.0, 0.0, 1)).xy();
    prev_1_position_ = (WorldTransform() * math::vec3(0.0, 0.0, 1)).xy();
    first_frame_ = false;
  }

  TransformableComponent::update(time);

  spawn_new_point_ = false;
  //check if distance and angle are large enough to spawn new trail points

  position_ = (WorldTransform() * math::vec3(0.0, 0.0, 1)).xy();


  auto p1_to_p2 = prev_1_position_ - last_position_;
  auto p1_to_p0 = position_ - last_position_;

  auto l1(p1_to_p2.Length());
  auto l2(p1_to_p0.Length());

  if (l2 > 0.0) {
    if (l1 > 0.0) {
      float angle = std::abs(math::vec2::DotProduct(p1_to_p0 / l2, p1_to_p2 / l1));
      if (angle < 0.9999) {
        prev_3_position_ = prev_2_position_;
        prev_2_position_ = prev_1_position_;
        prev_1_position_ = last_position_;
        last_position_ = position_;
        spawn_new_point_ = true;
      }
    } else {
      prev_3_position_ = prev_2_position_;
      prev_2_position_ = prev_1_position_;
      prev_1_position_ = last_position_;
      last_position_ = position_;
      spawn_new_point_ = true;
    }
  }
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
  result.Position = position_;
  result.LastPosition = last_position_;
  result.Prev1Position = prev_1_position_;
  result.Prev2Position = prev_2_position_;
  result.Prev3Position = prev_3_position_;
  result.SpawnNewPoint = spawn_new_point_;
  result.Self = this;

  return result;
}

////////////////////////////////////////////////////////////////////////////////

}
