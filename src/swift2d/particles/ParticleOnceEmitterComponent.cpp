////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleOnceEmitterComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleOnceEmitterComponent::ParticleOnceEmitterComponent()
  : ParticleSystem(nullptr)
  , Delay(0.f)
  , Amount(10.f)
  , DetachOnEmmission(false)
  , age_(0.0) {

  ParticleSystemLabel.on_change().connect([this](std::string const&) {
    this->ParticleSystem = nullptr;
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void ParticleOnceEmitterComponent::restart() {
  age_ = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

void ParticleOnceEmitterComponent::update(double time) {

  TransformableComponent::update(time);

  if (!ParticleSystem) {
    ParticleSystem = get_user()->get_component<ParticleSystemComponent>(ParticleSystemLabel());
  }

  if (ParticleSystem && age_ >= 0) {

    age_ += time;

    if (age_ > Delay()) {
      age_ = -1.0;

      auto pos(get_world_position());
      auto rot(get_world_rotation());

      ParticleSystem->spawn(pos, rot, Amount());

      if (DetachOnEmmission()) {
        detach();
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleOnceEmitterComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("ParticleSystemLabel", ParticleSystemLabel);
  visitor.add_member("Delay", Delay);
  visitor.add_member("Amount", Amount);
  visitor.add_member("DetachOnEmmission", DetachOnEmmission);
}

////////////////////////////////////////////////////////////////////////////////

}
