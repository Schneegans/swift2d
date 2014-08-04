////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/ParticleManager.hpp"

using namespace swift;

////////////////////////////////////////////////////////////////////////////////

ParticleManager::ParticleManager() {
  auto scene = SceneManager::instance()->get_default();

  auto particles_systems = scene->add_object(SceneObject::create_from_file(
    Application::instance()->get_resource("scene", "particle_systems.json")
  ));

  particles_systems_ = particles_systems->get_components<ParticleSystemComponent>();
}

////////////////////////////////////////////////////////////////////////////////

void ParticleManager::add_emitter(Type type, ParticleEmitterComponentPtr const& emitter) {
  particles_systems_[type]->add_emitter(emitter.get());
}

////////////////////////////////////////////////////////////////////////////////

void ParticleManager::remove_emitter(Type type, ParticleEmitterComponentPtr const& emitter) {
  particles_systems_[type]->remove_emitter(emitter.get());
}

////////////////////////////////////////////////////////////////////////////////
