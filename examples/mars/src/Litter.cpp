////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/Litter.hpp"

#include "../include/ParticleManager.hpp"
#include "../include/Fire.hpp"

bool Litter::assets_loaded_ = false;
ParticleSystemComponentPtr Litter::smoke_;

////////////////////////////////////////////////////////////////////////////////

Litter::Litter(math::vec2 const& pos, math::vec2 const& dir)
  : initialized_(false)
  , dir_(dir) {

  Transform = math::make_translation(pos) * math::make_scale(0.1);
}

////////////////////////////////////////////////////////////////////////////////

void Litter::update(double time) {

  SceneObject::update(time);

  if (!assets_loaded_) {
    assets_loaded_ = true;

    auto mat = Material::create();
    mat->Diffuse = Color(0, 0, 0, 1.0);
    mat->Emit = 1;
    mat->Glow = 0;
    MaterialDatabase::instance()->add("litter", mat);
  }

  if (!initialized_) {
    initialized_ = true;

    auto tex = add<SpriteComponent>();
    tex->Depth = 0.0f;
    tex->Material = MaterialDatabase::instance()->get("litter");

    auto smoke_emitter = add<ParticleEmitterComponent>();
         smoke_emitter->Density = 10;
         smoke_emitter->PositionVariance = 1;
         smoke_emitter->Life = 0.6;
         smoke_emitter->LifeVariance = 0.3;

    auto fire_emitter = add<ParticleEmitterComponent>();
         fire_emitter->Density = 30;
         fire_emitter->PositionVariance = 1;
         fire_emitter->Life = 0.3;
         fire_emitter->LifeVariance = 0.1;

    ParticleManager::instance()->add_emitter(ParticleManager::SMOKE, smoke_emitter);
    ParticleManager::instance()->add_emitter(ParticleManager::FIRE, fire_emitter);

    auto deleter = add<TimedDeleteBehavior>();
         deleter->Life = math::random::get(1.f, 2.f);
    deleter->on_delete.connect([=](){
      ParticleManager::instance()->remove_emitter(ParticleManager::SMOKE, smoke_emitter);
      ParticleManager::instance()->remove_emitter(ParticleManager::FIRE, fire_emitter);
    });

    auto shape = CircleCollisionShape::create();
    auto body = add<DynamicBodyComponent>();
    body->LinearDamping = 0;
    body->Density = 0.2;
    body->AngularDamping = 0;
    body->Shape = shape;
    body->start_contact_with_static.connect([this](StaticBodyComponent* other, math::vec2 const& pos) {
      Parent()->add_object(std::make_shared<Fire>(math::get_translation(WorldTransform())));
      detach();
    });

    body->apply_local_linear_impulse(dir_);

    SceneObject::update(0);
  }
}

////////////////////////////////////////////////////////////////////////////////
