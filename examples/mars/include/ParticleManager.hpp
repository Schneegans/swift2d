////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_PARTICLE_MANAGER_HPP
#define MARS_PARTICLE_MANAGER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>
#include <vector>

class ParticleManager : public swift::Singleton<ParticleManager> {
 public:

  enum Type {
    SMOKE,
    THICK_SMOKE,
    FIRE,
    HEAT
  };

  void add_emitter(Type type, swift::ParticleEmitterComponentPtr const& emitter);
  void remove_emitter(Type type, swift::ParticleEmitterComponentPtr const& emitter);

  friend class swift::Singleton<ParticleManager>;

 private:
  ParticleManager();

  std::vector<swift::ParticleSystemComponentPtr> particles_systems_;
};

// -----------------------------------------------------------------------------

#endif // MARS_PARTICLE_MANAGER_HPP
