////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/trails/TrailEmitterComponent.hpp>

#include <swift2d/trails/TrailSystemComponent.hpp>

#include <swift2d/materials/Material.hpp>
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/components/SpriteComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TrailEmitterComponent::TrailEmitterComponent()
  : TrailSystem(nullptr)
  , MinSpawnGap(0.1f)
  , MaxSpawnGap(10.f)
  , Life(1.0f)
  , StartAge(0.f)
  , Paused(false)
  , time_since_last_spawn_(0.f)
  , time_since_prev_1_spawn_(0.f)
  , time_since_prev_2_spawn_(0.f)
  , prev_1_position_()
  , prev_2_position_()
  , prev_3_position_()
  , prev_4_position_()
  , first_frame_(true) {

  TrailSystem.before_change().connect([this](TrailSystemComponentPtr const& val) {
    if (val) {
      val->remove_emitter(this);
    }
    return true;
  });

  TrailSystem.on_change().connect([this](TrailSystemComponentPtr const& val) {
    if (val) {
      val->add_emitter(this);
    }
    return true;
  });

  TrailSystemLabel.on_change().connect([this](std::string const&) {
    this->TrailSystem = nullptr;
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

TrailEmitterComponent::~TrailEmitterComponent() {
  if (TrailSystem()) {
    TrailSystem()->remove_emitter(this);
  }
}

////////////////////////////////////////////////////////////////////////////////

void TrailEmitterComponent::on_detach(double time) {
  update(time);
  spawn_segment();
}

////////////////////////////////////////////////////////////////////////////////

void TrailEmitterComponent::update(double time) {

  TransformableComponent::update(time);

  if (!TrailSystem()) {
    TrailSystem = get_user()->get_component<TrailSystemComponent>(TrailSystemLabel());
  }

  if (first_frame_) {
    prev_1_position_   = (WorldTransform() * math::vec3(0.0, 0.0, 1)).xy();
    prev_4_position_ = prev_1_position_;
    prev_3_position_ = prev_1_position_;
    prev_2_position_ = prev_1_position_;
    first_frame_     = false;
  }

  time_since_last_spawn_   += time;
  time_since_prev_1_spawn_ += time;
  time_since_prev_2_spawn_ += time;

  //check if distance and angle are large enough to spawn new trail points
  auto p1_to_p2 = prev_2_position_ - prev_1_position_;
  auto p1_to_p0 = get_world_position() - prev_1_position_;

  auto l1(p1_to_p2.Length());
  auto l2(p1_to_p0.Length());

  if (l2 > MinSpawnGap() && l1 > 0.0) {
    float angle = std::abs(math::dot(p1_to_p0 / l2, p1_to_p2 / l1));
    if (angle < 0.9999) {
      spawn_segment();
    }
  }

  if (time_since_last_spawn_ > 0.f && (l2 > MaxSpawnGap())) {
    spawn_segment();
  }
}

////////////////////////////////////////////////////////////////////////////////

void TrailEmitterComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("TrailSystemLabel",  TrailSystemLabel);
  visitor.add_member("MinSpawnGap",  MinSpawnGap);
  visitor.add_member("MaxSpawnGap",  MaxSpawnGap);
  visitor.add_member("Life",         Life);
  visitor.add_member("StartAge",     StartAge);
}

////////////////////////////////////////////////////////////////////////////////

void TrailEmitterComponent::spawn_segment() {
  if (TrailSystem()) {
    prev_4_position_ = prev_3_position_;
    prev_3_position_ = prev_2_position_;
    prev_2_position_ = prev_1_position_;
    prev_1_position_ = get_world_position();
    time_since_prev_2_spawn_ = time_since_prev_1_spawn_;
    time_since_prev_1_spawn_ = time_since_last_spawn_;
    time_since_last_spawn_   = 0.f;

    if (!Paused()) {
      TrailSystem()->spawn(make_end_segment());
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

TrailSegment TrailEmitterComponent::make_end_segment() const {
  TrailSegment result;

  result.Position             = get_world_position();
  result.TimeSinceLastSpawn   = time_since_last_spawn_;
  result.TimeSincePrev1Spawn  = time_since_prev_1_spawn_;
  result.TimeSincePrev2Spawn  = time_since_prev_2_spawn_;
  result.Life                 = Life();
  result.StartAge             = StartAge();
  result.Prev1Position        = prev_1_position_;
  result.Prev2Position        = prev_2_position_;
  result.Prev3Position        = prev_3_position_;
  result.Prev4Position        = prev_4_position_;

  return result;
}

////////////////////////////////////////////////////////////////////////////////

}
