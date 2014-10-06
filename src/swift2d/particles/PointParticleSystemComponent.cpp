////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/PointParticleSystemComponent.hpp>

#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointParticleSystemComponent::PointParticleSystemComponent()
  : Scale(1.f)
  , StartGlow(0.f),                EndGlow(0.f)
  , StartColor(Color(1, 1, 1, 1)), EndColor(Color(1, 1, 1, 0))
  , BlendAdd(false) {}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.Scale = Scale();
  s.StartGlow = StartGlow();
  s.EndGlow = EndGlow();
  s.StartColor = StartColor().vec4();
  s.EndColor = EndColor().vec4();
  s.BlendAdd = BlendAdd();

  scene->renderers().point_particle_system_renderer.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("Scale",       Scale);
  visitor.add_member("StartGlow",   StartGlow);
  visitor.add_member("EndGlow",     EndGlow);
  visitor.add_member("StartColor",  StartColor);
  visitor.add_member("EndColor",    EndColor);
  visitor.add_member("BlendAdd",    BlendAdd);
}

////////////////////////////////////////////////////////////////////////////////

}
