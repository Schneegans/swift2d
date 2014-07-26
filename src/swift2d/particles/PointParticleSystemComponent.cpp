////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/PointParticleSystemComponent.hpp>

#include <swift2d/particles/PointParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointParticleSystemComponent::PointParticleSystemComponent()
  : Scale(1.f)
  , StartGlow(0.f)
  , EndGlow(0.f)
  , StartColor(Color(1.f, 1.f, 1.f, 1.f))
  , EndColor(Color(1.f, 1.f, 1.f, 0.f)) {}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::draw(RenderContext const& ctx) {
  ParticleSystemComponent::update_particles(ctx);

  ctx.gl.Rasterization::PointSize(Scale());

  auto shader(PointParticleShader::instance());
  shader->use(ctx);
  shader->set_uniform("projection", ctx.projection_matrix);
  shader->set_uniform("start_color", StartColor().vec4());
  shader->set_uniform("end_color", EndColor().vec4());
  shader->set_uniform("glow", math::vec2(StartGlow(), EndGlow()));

  ParticleSystemComponent::draw_particles(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("Scale", Scale);
  visitor.add_member("StartGlow", StartGlow);
  visitor.add_member("EndGlow", EndGlow);
  visitor.add_member("StartColor", StartColor);
  visitor.add_member("EndColor", EndColor);
}

////////////////////////////////////////////////////////////////////////////////

}
