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
  , DamageSourceResetTime(1)
  , Invulnerable(false)
  , damage_source_(0)
  , dead_(false) {}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::increase(float amount) {
  if (!Invulnerable()) {
    Life = std::min(Life() + amount, MaxLife());

    if (dead_ && Life() > 0.f) {
      dead_ = false;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::decrease(float amount) {
  if (!Invulnerable()) {
    Life = std::max(Life() - amount, 0.f);
  }
}

////////////////////////////////////////////////////////////////////////////////

void LifeComponent::decrease(float amount, math::uint64 source) {
  if (!Invulnerable()) {
    Life = std::max(Life() - amount, 0.f);
    damage_source_ = source;
    reset_time_ = DamageSourceResetTime();
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
  visitor.add_member("Invulnerable", Invulnerable);
}

////////////////////////////////////////////////////////////////////////////////

}
