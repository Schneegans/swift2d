////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_CPU_PARTICLE_SYSTEM_SHADER_HPP
#define SWIFT2D_CPU_PARTICLE_SYSTEM_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class CPUParticleSystemShader : public Shader,
                                public Singleton<CPUParticleSystemShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  friend class Singleton<CPUParticleSystemShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  CPUParticleSystemShader();
  ~CPUParticleSystemShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_CPU_PARTICLE_SYSTEM_SHADER_HPP
