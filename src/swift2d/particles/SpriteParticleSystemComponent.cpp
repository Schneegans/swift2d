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
  : StartScale(1.f), EndScale(1.f) {}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::draw(RenderContext const& ctx) {
  ParticleSystemComponent::update_particles(ctx);

  Texture()->bind(ctx, 0);

  auto shader(SpriteParticleShader::instance());
  shader->use(ctx);
  shader->set_uniform("projection", ctx.projection_matrix);
  shader->set_uniform("diffuse", 0);
  shader->set_uniform("scale", math::vec2(StartScale(), EndScale()));

  ParticleSystemComponent::draw_particles(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("StartScale", StartScale);
  visitor.add_member("EndScale", EndScale);
  visitor.add_object("Texture", Texture);
}

////////////////////////////////////////////////////////////////////////////////

}
