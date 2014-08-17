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
#include <swift2d/graphics/Pipeline.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteParticleSystemComponent::SpriteParticleSystemComponent()
  : StartScale(1.f),               EndScale(1.f)
  , StartGlow(0.f),                EndGlow(0.f)
  , StartColor(Color(1, 1, 1, 1)), EndColor(Color(1, 1, 1, 0))
  , BlendAdd(false)
  , SubSamplingLevel(1) {}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::draw(RenderContext const& ctx) {
  ParticleSystemComponent::update_particles(ctx);

  if (SubSamplingLevel() > 1) {
    ctx.pipeline->get_sub_sampler(SubSamplingLevel())->bind(ctx, BlendAdd());
  } else if (BlendAdd()) {
    ctx.gl.BlendFunc(ose::SrcAlpha(), ose::One());
  }

  Texture()->bind(ctx, 0);

  auto& shader(SpriteParticleShader::get());
  shader.use(ctx);
  shader.projection. Set(ctx.projection_matrix);
  shader.diffuse.    Set(0);
  shader.scale.      Set(math::vec2(StartScale(), EndScale()));
  shader.glow.       Set(math::vec2(StartGlow(), EndGlow()));
  shader.start_color.Set(StartColor().vec4());
  shader.end_color.  Set(EndColor().vec4());

  ParticleSystemComponent::draw_particles(ctx);

  if (SubSamplingLevel() > 1) {
    ctx.pipeline->get_sub_sampler(SubSamplingLevel())->draw(ctx, BlendAdd());
  } else if (BlendAdd()) {
    ctx.gl.BlendFunc(ose::SrcAlpha(), ose::OneMinusSrcAlpha());
  }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  ParticleSystemComponent::serialize(scene);
  scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("StartScale",  StartScale);
  visitor.add_member("EndScale",    EndScale);
  visitor.add_member("StartGlow",   StartGlow);
  visitor.add_member("EndGlow",     EndGlow);
  visitor.add_member("StartColor",  StartColor);
  visitor.add_member("EndColor",    EndColor);
  visitor.add_member("BlendAdd",    BlendAdd);
  visitor.add_member("SubSamplingLevel", SubSamplingLevel);
  visitor.add_object("Texture",     Texture);
}

////////////////////////////////////////////////////////////////////////////////

}
