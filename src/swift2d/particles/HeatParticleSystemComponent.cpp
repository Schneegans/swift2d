////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/HeatParticleSystemComponent.hpp>

#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HeatParticleSystemComponent::HeatParticleSystemComponent()
  : StartScale(1.f),    EndScale(1.f)
  , StartOpacity(1.f),  EndOpacity(0.f) {}

////////////////////////////////////////////////////////////////////////////////

void HeatParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.StartScale = StartScale();
  s.EndScale = EndScale();
  s.StartOpacity = StartOpacity();
  s.EndOpacity = EndOpacity();
  s.Texture = Texture();

  scene->renderers().heat_particle_system_renderer.add(std::move(s));
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
