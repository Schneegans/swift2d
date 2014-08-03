////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/Litter.hpp"

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
    mat->Diffuse = Color(1, 0.8, 0.3, 1.0);
    mat->Emit = 1;
    mat->Glow = 1;
    MaterialDatabase::instance()->add("litter", mat);

    // smoke_ = SceneManager::instance()->get_default()->get_component<ParticleSystemComponent>();
  }

  if (!initialized_) {
    initialized_ = true;

    auto tex = add<SpriteComponent>();
    tex->Depth = 0.0f;
    tex->Material = MaterialDatabase::instance()->get("litter");

    // auto emitter = add<ParticleEmitterComponent>();
    //      emitter->Density = 20;
    //      emitter->PositionVariance = 1;
    //      emitter->Life = 1;
    //      emitter->LifeVariance = 0.7;
    // smoke_->add_emitter(emitter.get());

    auto deleter = add<TimedDeleteBehavior>();
         deleter->Life = math::random::get(1.f, 2.f);
    // deleter->on_delete.connect([=](){
    //   smoke_->remove_emitter(emitter.get());
    // });

    auto shape = CircleCollisionShape::create();
    auto body = add<DynamicBodyComponent>();
    body->LinearDamping = 0;
    body->Density = 0.2;
    body->AngularDamping = 0;
    body->Shape = shape;

    body->apply_local_linear_impulse(dir_);

    SceneObject::update(0);
  }
}

////////////////////////////////////////////////////////////////////////////////
