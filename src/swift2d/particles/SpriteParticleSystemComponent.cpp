////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/SpriteParticleSystemComponent.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteParticleSystemComponent::SpriteParticleSystemComponent()
  : StartScale(math::vec2(1.f, 1.f)), EndScale(math::vec2(1.f, 1.f))
  , StartGlow(0.f),                   EndGlow(0.f)
  , StartColor(Color(1, 1, 1, 1)),    EndColor(Color(1, 1, 1, 0))
  , BlendAdd(false)
  , SubSamplingLevel(1) {}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.StartScale = StartScale();
  s.EndScale = EndScale();
  s.StartGlow = StartGlow();
  s.EndGlow = EndGlow();
  s.StartColor = StartColor().vec4();
  s.EndColor = EndColor().vec4();
  s.BlendAdd = BlendAdd();
  s.SubSamplingLevel = SubSamplingLevel();
  s.Texture = Texture();

  scene->renderers().sprite_particle_system_renderer.add(std::move(s));
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
