////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "Mover.hpp"

////////////////////////////////////////////////////////////////////////////////

Mover::Mover() {
  auto w = WindowManager::instance()->get_default();
  w->on_key_press.connect([&](Key key, int scancode, int action, int mods){
    if (action == 0) {
      if (key == Key::W) {
        get_user()->get_components<ParticleSystemComponent>()[0]->Emitter()->Density = 0.0;
        get_user()->get_components<ParticleSystemComponent>()[1]->Emitter()->Density = 0.0;
        get_user()->get_components<ParticleSystemComponent>()[2]->Emitter()->Density = 0.0;
        get_user()->get_components<ParticleSystemComponent>()[3]->Emitter()->Density = 0.0;
        get_user()->get_components<ParticleSystemComponent>()[4]->Emitter()->Density = 0.0;
      }
    } else if (action == 1) {
      if (key == Key::W) {
        get_user()->get_components<ParticleSystemComponent>()[0]->Emitter()->Density = 100.0;
        get_user()->get_components<ParticleSystemComponent>()[1]->Emitter()->Density = 100.0;
        get_user()->get_components<ParticleSystemComponent>()[2]->Emitter()->Density = 50.0;
        get_user()->get_components<ParticleSystemComponent>()[3]->Emitter()->Density = 15.0;
        get_user()->get_components<ParticleSystemComponent>()[4]->Emitter()->Density = 100.0;
      }
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

void Mover::update(double time) {
  if (camera_) {
    auto mat = camera_->Transform();
    math::set_translation(mat, math::get_translation(get_user()->WorldTransform()));
    camera_->Transform = mat;
  }

  auto w = WindowManager::instance()->get_default();
  auto c = get_user()->get_component<DynamicBodyComponent>();

  if(w->key_pressed(Key::W)) {
    if (c->get_linear_velocity().Length() < 3.f) {
      c->apply_local_force(math::vec2(0.075f, 0.f));
    }
  }
  if(w->key_pressed(Key::D)) {
    if (c->get_angular_velocity() > -5.f) {
      c->apply_torque(-0.01f);
    }
  }
  if(w->key_pressed(Key::A)) {
    if (c->get_angular_velocity() < 5.f) {
      c->apply_torque(0.01f);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////



