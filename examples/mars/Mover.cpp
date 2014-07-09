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
        LinearSpeed.set(0);
        get_user()->get_components<ParticleSystemComponent>()[0]->Emitter()->Density = 0.0;
        get_user()->get_components<ParticleSystemComponent>()[1]->Emitter()->Density = 0.0;
        get_user()->get_components<ParticleSystemComponent>()[2]->Emitter()->Density = 0.0;
        get_user()->get_components<ParticleSystemComponent>()[3]->Emitter()->Density = 0.0;
      }
      if (key == Key::S) LinearSpeed.set(0);
      if (key == Key::A) AngularSpeed.set(0);
      if (key == Key::D) AngularSpeed.set(0);
    } else if (action == 1) {
      if (key == Key::W) {
        LinearSpeed.set( 10);
        get_user()->get_components<ParticleSystemComponent>()[0]->Emitter()->Density = 100.0;
        get_user()->get_components<ParticleSystemComponent>()[1]->Emitter()->Density = 100.0;
        get_user()->get_components<ParticleSystemComponent>()[2]->Emitter()->Density = 50.0;
        get_user()->get_components<ParticleSystemComponent>()[3]->Emitter()->Density = 15.0;
      }
      if (key == Key::S) LinearSpeed.set(-10);
      if (key == Key::A) AngularSpeed.set(-2 );
      if (key == Key::D) AngularSpeed.set( 2 );
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

void Mover::update(double time) {
  MoveBehavior::update(time);

  if (camera_) {
    auto mat = camera_->Transform();
    math::set_translate(mat, math::get_translate(get_user()->WorldTransform()));
    camera_->Transform = mat;
  }
}

////////////////////////////////////////////////////////////////////////////////



