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
  : MinSpawnGap       (0.1f)
  , MaxSpawnGap       (10.f)
  , position_()
  , time_since_last_spawn_(0.f)
  , time_since_prev_1_spawn_(0.f)
  , time_since_prev_2_spawn_(0.f)
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

  time_since_last_spawn_ += time;
  time_since_prev_1_spawn_ += time;
  time_since_prev_2_spawn_ += time;

  TransformableComponent::update(time);

  //check if distance and angle are large enough to spawn new trail points
  position_ = (WorldTransform() * math::vec3(0.0, 0.0, 1)).xy();

  auto p1_to_p2 = prev_1_position_ - last_position_;
  auto p1_to_p0 = position_ - last_position_;

  auto l1(p1_to_p2.Length());
  auto l2(p1_to_p0.Length());

  auto spawn_point = [&](){
    prev_3_position_ = prev_2_position_;
    prev_2_position_ = prev_1_position_;
    prev_1_position_ = last_position_;
    last_position_ = position_;
    spawn_new_point_ = true;
    time_since_prev_2_spawn_ = time_since_prev_1_spawn_;
    time_since_prev_1_spawn_ = time_since_last_spawn_;
    time_since_last_spawn_ = 0.f;
  };

  if (l2 > MinSpawnGap() && l1 > 0.0) {
    float angle = std::abs(math::dot(p1_to_p0 / l2, p1_to_p2 / l1));
    if (angle < 0.9999) {
      spawn_point();
    }
  }

  if ((!spawn_new_point_) && (l2 > MaxSpawnGap())) {
    spawn_point();
  }
}

////////////////////////////////////////////////////////////////////////////////

void TrailEmitterComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("MinSpawnGap",   MinSpawnGap);
  visitor.add_member("MaxSpawnGap",   MaxSpawnGap);
}

////////////////////////////////////////////////////////////////////////////////

SerializedTrailEmitter TrailEmitterComponent::make_serialized_emitter() const {
  SerializedTrailEmitter result;

  result.Position = position_;
  result.TimeSinceLastSpawn = time_since_last_spawn_;
  result.TimeSincePrev1Spawn = time_since_prev_1_spawn_;
  result.TimeSincePrev2Spawn = time_since_prev_2_spawn_;
  result.LastPosition = last_position_;
  result.Prev1Position = prev_1_position_;
  result.Prev2Position = prev_2_position_;
  result.Prev3Position = prev_3_position_;
  result.SpawnNewPoint = spawn_new_point_;

  spawn_new_point_ = false;

  return result;
}

////////////////////////////////////////////////////////////////////////////////

}
