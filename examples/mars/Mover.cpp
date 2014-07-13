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

Mover::Mover()
 : camera_zoom_(1.f)
 , camera_offset_(0.f, 0.f) {

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

  auto c = get_user()->get_component<DynamicBodyComponent>();

  if (camera_) {

    float target_zoom(c->get_linear_velocity().Length()*0.05 + 0.9);
    camera_zoom_ += (target_zoom - camera_zoom_) * time;

    math::vec2 target_offset(c->get_linear_velocity() * 0.5);
    camera_offset_ += (target_offset - camera_offset_) * time;

    auto mat(math::make_translation(camera_offset_ + math::get_translation(get_user()->WorldTransform())));
    mat = mat*math::make_scale(camera_zoom_);
    camera_->Transform = mat;
  }

  auto w = WindowManager::instance()->get_default();

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



