////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/Mover.hpp"

////////////////////////////////////////////////////////////////////////////////

Mover::Mover()
 : camera_zoom_(1.f)
 , camera_offset_(0.f, 0.f) {

  auto w = WindowManager::instance()->get_default();
  w->on_key_press.connect([&](Key key, int scancode, int action, int mods){
    if (action == 0) {
      if (key == Key::W) {

        for (auto emitter: get_user()->get_objects()) {
          if (emitter->Label == "Smoke" || emitter->Label == "Sparks" ||
              emitter->Label == "Fire"  || emitter->Label == "Light" ||
              emitter->Label == "Heat") {
            emitter->get_component<ParticleEmitterComponent>()->Density = 0.0;
          }
        }

      }
    } else if (action == 1) {
      if (key == Key::W) {

        for (auto emitter: get_user()->get_objects()) {
          if (emitter->Label == "Smoke") {
            emitter->get_component<ParticleEmitterComponent>()->Density = 150.0;
          } else if (emitter->Label == "Sparks") {
            emitter->get_component<ParticleEmitterComponent>()->Density = 250.0;
          } else if (emitter->Label == "Fire") {
            emitter->get_component<ParticleEmitterComponent>()->Density = 100.0;
          } else if (emitter->Label == "Light") {
            emitter->get_component<ParticleEmitterComponent>()->Density = 10.0;
          } else if (emitter->Label == "Heat") {
            emitter->get_component<ParticleEmitterComponent>()->Density = 100.0;
          }
        }
      }
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

void Mover::update(double time) {

  auto c = get_user()->get_component<DynamicBodyComponent>();
  auto speed(c->get_linear_velocity().Length());

  if (camera_) {

    float target_zoom(speed*0.02 + 0.8);
    camera_zoom_ += (target_zoom - camera_zoom_) * time * 0.5;

    math::vec2 target_offset(c->get_linear_velocity() * 0.75);
    camera_offset_ += (target_offset - camera_offset_) * time * 0.5;

    auto mat(math::make_translation(camera_offset_ + math::get_translation(get_user()->WorldTransform())));
    mat = mat*math::make_scale(camera_zoom_);
    camera_->Transform = mat;
  }

  auto w = WindowManager::instance()->get_default();

  if(w->key_pressed(Key::W)) {
    if (speed < 7.5f) {
      c->apply_local_force(math::vec2(5.5f, 0.f));
    }
  }

  if(w->key_pressed(Key::D)) {
    if (c->get_angular_velocity() > -5.f) {
      c->apply_torque(-5.f);
    }
  }
  if(w->key_pressed(Key::A)) {
    if (c->get_angular_velocity() < 5.f) {
      c->apply_torque(5.f);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////



