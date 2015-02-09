////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/components/LifeComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LifeComponent::LifeComponent()
  : Life(100)
  , MaxLife(100)
  , AccumulationDelay(0.1)
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

    accumulated_damage_ += amount;
    accumulated_abs_damage_ += std::abs(amount);
    accumulated_direction_ += direction * (accumulated_abs_damage_ > 0 ? std::min(1.f, std::abs(amount)/accumulated_abs_damage_) : 1);
    if (accumulation_time_ < 0) {
      accumulation_time_ = AccumulationDelay();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::decrease(float amount, math::vec2 const& direction) {
  if (!Invulnerable()) {
    Life = std::max(Life() - amount, 0.f);

    accumulated_damage_ -= amount;
    accumulated_abs_damage_ += std::abs(amount);
    accumulated_direction_ += direction * (accumulated_abs_damage_ > 0 ? std::min(1.f, std::abs(amount)/accumulated_abs_damage_) : 1);
    if (accumulation_time_ < 0) {
      accumulation_time_ = AccumulationDelay();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::decrease(float amount, math::uint64 source, math::vec2 const& direction) {
  if (!Invulnerable()) {
    Life = std::max(Life() - amount, 0.f);
    damage_source_ = source;
    reset_time_ = DamageSourceResetTime();

    accumulated_damage_ -= amount;
    accumulated_abs_damage_ += std::abs(amount);
    accumulated_direction_ += direction * (accumulated_abs_damage_ > 0 ? std::min(1.f, std::abs(amount)/accumulated_abs_damage_) : 1);
    if (accumulation_time_ < 0) {
      accumulation_time_ = AccumulationDelay();
    }
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

  if (accumulation_time_ > 0.f) {
    accumulation_time_ -= time;

    if (accumulation_time_ <= 0.f) {
      accumulation_time_ = -1.f;
      on_life_change.emit(accumulated_damage_, accumulated_direction_);
      accumulated_damage_ = 0;
      accumulated_abs_damage_ = 0;
      accumulated_direction_ = math::vec2();
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
  visitor.add_member("Invulnerable", Invulnerable);
}

////////////////////////////////////////////////////////////////////////////////

}
