////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleEmitterComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleEmitterComponent::ParticleEmitterComponent()
  : Density                      (5.f) {}

////////////////////////////////////////////////////////////////////////////////

void ParticleEmitterComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Density",                   Density);
}

////////////////////////////////////////////////////////////////////////////////

SerializedEmitter ParticleEmitterComponent::make_serialized_emitter() const {
  SerializedEmitter result;
  auto pos(math::get_translation(WorldTransform()));
  auto rot(math::get_rotation(WorldTransform()));
  result.Density = Density();
  result.PosRot = math::vec3(pos.x(), pos.y(), rot);
  result.Self = this;

  return result;
}

////////////////////////////////////////////////////////////////////////////////

}
