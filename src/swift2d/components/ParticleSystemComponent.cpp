////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/ParticleSystemComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleSystemComponent::ParticleSystemComponent()
  : Depth(0.f)
  , particle_system_(ParticleSystem::create(this)) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::update(double time) {
  DrawableComponent::update(time);
  particle_system_->update(time);
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::draw(RenderContext const& ctx) {
  particle_system_->draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

int ParticleSystemComponent::get_particle_count() const {
  return 0;
  // return particle_system_->get_particle_count();
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  switch (Emitter()->target()) {
    case SerializedScene::OBJECTS:
      scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
      break;
    case SerializedScene::LIGHTS:
      scene->lights.insert(std::make_pair(Depth.get(), create_copy()));
      break;
    case SerializedScene::HEAT_OBJECTS:
      scene->heat_objects.insert(std::make_pair(Depth.get(), create_copy()));
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  DrawableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_object("Emitter", Emitter);
}

////////////////////////////////////////////////////////////////////////////////

}
