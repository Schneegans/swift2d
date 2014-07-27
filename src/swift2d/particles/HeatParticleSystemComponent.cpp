////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/HeatParticleSystemComponent.hpp>

#include <swift2d/particles/HeatParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HeatParticleSystemComponent::HeatParticleSystemComponent()
  : StartScale(1.f),    EndScale(1.f)
  , StartOpacity(1.f),  EndOpacity(0.f) {}

////////////////////////////////////////////////////////////////////////////////

void HeatParticleSystemComponent::draw(RenderContext const& ctx) {
  ParticleSystemComponent::update_particles(ctx);

  Texture()->bind(ctx, 0);

  auto shader(HeatParticleShader::instance());
  shader->use(ctx);
  shader->projection. Set(ctx.projection_matrix);
  shader->diffuse.    Set(0);
  shader->scale.      Set(math::vec2(StartScale(), EndScale()));
  shader->opacity.    Set(math::vec2(StartOpacity(), EndOpacity()));

  ParticleSystemComponent::draw_particles(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void HeatParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  ParticleSystemComponent::serialize(scene);
  scene->heat_objects.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void HeatParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("StartScale",    StartScale);
  visitor.add_member("EndScale",      EndScale);
  visitor.add_member("StartOpacity",  StartOpacity);
  visitor.add_member("EndOpacity",    EndOpacity);
  visitor.add_object("Texture",       Texture);
}

////////////////////////////////////////////////////////////////////////////////

}
