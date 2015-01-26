////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
  , DetachOnEmmission(true) {

  ParticleSystemLabel.on_change().connect([this](std::string const&) {
    ParticleSystem = nullptr;
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void ParticleOnceEmitterComponent::update(double time) {

  TransformableComponent::update(time);

  if (!ParticleSystem) {
    ParticleSystem = get_user()->get_component<ParticleSystemComponent>(ParticleSystemLabel());
  }

  if (ParticleSystem && Delay() >= 0) {

    Delay = Delay() - time;

    if (Delay() <= 0.0) {
      Delay = -1.0;

      auto pos(get_world_position());
      auto rot(get_world_rotation());

      ParticleSystem->spawn(math::vec3(pos.x(), pos.y(), rot), Amount());

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
