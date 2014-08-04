////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/Fire.hpp"

#include "../include/ParticleManager.hpp"

////////////////////////////////////////////////////////////////////////////////

Fire::Fire(math::vec2 const& pos)
  : initialized_(false) {

  Transform = math::make_translation(pos) * math::make_scale(0.5);
}

////////////////////////////////////////////////////////////////////////////////

void Fire::update(double time) {

  if (!initialized_) {
    initialized_ = true;

    auto smoke_emitter = add<ParticleEmitterComponent>();
         smoke_emitter->Density = 5;
         smoke_emitter->PositionVariance = 0;
         smoke_emitter->Life = 1.5;
         smoke_emitter->LifeVariance = 1.0;

    auto fire_emitter = add<ParticleEmitterComponent>();
         fire_emitter->Density = 5;
         fire_emitter->PositionVariance = 0.2;
         fire_emitter->Life = 0.7;
         fire_emitter->LifeVariance = 0.1;

    auto deleter = add<TimedDeleteBehavior>();
         deleter->Life = math::random::get(3.f, 7.f);

    ParticleManager::instance()->add_emitter(ParticleManager::SMOKE, smoke_emitter);
    ParticleManager::instance()->add_emitter(ParticleManager::FIRE, fire_emitter);
    ParticleManager::instance()->add_emitter(ParticleManager::HEAT, fire_emitter);

    deleter->on_delete.connect([=](){
      ParticleManager::instance()->remove_emitter(ParticleManager::SMOKE, smoke_emitter);
      ParticleManager::instance()->remove_emitter(ParticleManager::FIRE, fire_emitter);
      ParticleManager::instance()->remove_emitter(ParticleManager::HEAT, fire_emitter);
    });
  }

  SceneObject::update(time);
}

////////////////////////////////////////////////////////////////////////////////

void Fire::accept(SavableObjectVisitor& visitor) {
  SceneObject::accept(visitor);
}

////////////////////////////////////////////////////////////////////////////////
