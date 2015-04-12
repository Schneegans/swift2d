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

#include <swift2d/components/LifeComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LifeComponent::LifeComponent()
  : Life(100)
  , MaxLife(100)
  , AccumulationDelay(0.1)
  , AccumulationThreshold(10)
  , DamageSourceResetTime(1)
  , Invulnerable(false)
  , damage_source_(0)
  , dead_(false)
  , accumulation_time_(-1.f)
  , accumulated_damage_(0)
  , accumulated_abs_damage_(0) {}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::increase(float amount, math::vec2 const& direction) {
  if (!Invulnerable()) {
    Life = std::min(Life() + amount, MaxLife());

    if (dead_ && Life() > 0.f) {
      dead_ = false;
    }

    accumulate_damage(amount, direction);
  }
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::decrease(float amount, math::vec2 const& direction) {
  if (!Invulnerable()) {
    Life = std::max(Life() - amount, 0.f);
    accumulate_damage(-amount, direction);
  }
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::decrease(float amount, math::uint64 source, math::vec2 const& direction) {
  if (!Invulnerable()) {
    Life = std::max(Life() - amount, 0.f);

    if (accumulated_damage_ > 0 && damage_source_ != source) {
      emit_accumulated_damage();
    }

    damage_source_ = source;
    reset_time_ = DamageSourceResetTime();

    accumulate_damage(-amount, direction);
  }
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::reset() {
  Life = MaxLife();
  dead_ = false;
}

////////////////////////////////////////////////////////////////////////////////

bool LifeComponent::is_dead() const {
  return Life() == 0;
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::update(double time) {

  if (reset_time_ > 0.f) {
    reset_time_ -= time;

    if (reset_time_ <= 0.f) {
      reset_time_ = 0.f;
      damage_source_ = 0;
    }
  }

  if (accumulation_time_ >= 0.f) {
    accumulation_time_ -= time;

    if (accumulation_time_ <= 0.f || Life() <= 0.f) {
      emit_accumulated_damage();
    }
  }

  if (!dead_ && Life() <= 0.f) {
    dead_ = true;
    on_killed.emit(damage_source_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("Life", Life);
  visitor.add_member("MaxLife", MaxLife);
  visitor.add_member("DamageSourceResetTime", DamageSourceResetTime);
  visitor.add_member("AccumulationDelay", AccumulationDelay);
  visitor.add_member("AccumulationThreshold", AccumulationThreshold);
  visitor.add_member("Invulnerable", Invulnerable);
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::accumulate_damage(float amount, math::vec2 const& dir) {
  accumulated_damage_ += amount;
  accumulated_abs_damage_ += std::abs(amount);
  accumulated_direction_ += dir * (accumulated_abs_damage_ > 0 ? std::min(1.f, std::abs(amount)/accumulated_abs_damage_) : 1);
  if (accumulated_abs_damage_ >= AccumulationThreshold()) {
    accumulation_time_ = 0.f;
  }
  if (accumulation_time_ < 0) {
    accumulation_time_ = AccumulationDelay();
  }
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::emit_accumulated_damage() {
  accumulation_time_ = -1.f;
  on_life_change.emit(damage_source_, accumulated_damage_, accumulated_direction_);
  accumulated_damage_ = 0;
  accumulated_abs_damage_ = 0;
  accumulated_direction_ = math::vec2();
}

////////////////////////////////////////////////////////////////////////////////

}
