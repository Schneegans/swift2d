////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleEmitterComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleEmitterComponent::ParticleEmitterComponent()
  : ParticleSystem(nullptr)
  , Density (1.f)
  , particles_to_spawn_(0.f) {

  ParticleSystemLabel.on_change().connect([this](std::string const&) {
    ParticleSystem = nullptr;
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void ParticleEmitterComponent::update(double time) {

  auto last_spawn_position = math::get_translation(WorldTransform());

  TransformableComponent::update(time);
  Density.update(time);

  if (!ParticleSystem) {
    ParticleSystem = get_user()->get_component<ParticleSystemComponent>(ParticleSystemLabel());
  }

  if (ParticleSystem) {
    particles_to_spawn_ += time * Density();

    if (particles_to_spawn_ > 1.f) {
      auto pos(math::get_translation(WorldTransform()));
      auto rot(math::get_rotation(WorldTransform()));

      int count = particles_to_spawn_;
      particles_to_spawn_ = particles_to_spawn_ - count;

      for (int i(0); i<count; ++i) {
        auto p = pos + 1.f*i/count*(last_spawn_position-pos);
        ParticleSystem->spawn(math::vec3(p.x(), p.y(), rot));
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleEmitterComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Density", Density);
  visitor.add_member("ParticleSystemLabel", ParticleSystemLabel);
}

////////////////////////////////////////////////////////////////////////////////

}
