////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleEffect.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleEffect::ParticleEffect()
  : SceneObject()
  , Time (0.f) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleEffect::on_init() {

}

////////////////////////////////////////////////////////////////////////////////

void ParticleEffect::update(double time) {
  Time.update(time);
  SceneObject::update(time);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleEffect::accept(SavableObjectVisitor& visitor) {
  SceneObject::accept(visitor);
  visitor.add_member("Time", Time);
}

////////////////////////////////////////////////////////////////////////////////

}
