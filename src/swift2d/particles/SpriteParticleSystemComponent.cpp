////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/SpriteParticleSystemComponent.hpp>

#include <swift2d/particles/SpriteParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteParticleSystemComponent::SpriteParticleSystemComponent()
  : Depth(0.f)
  , particle_system_(ParticleSystem::create()) {}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::add_emitter(ParticleEmitterComponentPtr const& emitter) {
  emitters_.insert(emitter);
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::remove_emitter(ParticleEmitterComponentPtr const& emitter) {
  emitters_.erase(emitter);
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::update(double time) {
  DrawableComponent::update(time);
  particle_system_->update(emitters_, time);
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::draw(RenderContext const& ctx) {
  particle_system_->update_particles(emitters_, ctx);


  Texture()->bind(ctx, 0);

  auto shader(SpriteParticleShader::instance());
  shader->use(ctx);
  shader->set_uniform("projection", ctx.projection_matrix);
  shader->set_uniform("diffuse", 0);

  particle_system_->draw_particles(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  DrawableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_object("Texture", Texture);
}

////////////////////////////////////////////////////////////////////////////////

}
