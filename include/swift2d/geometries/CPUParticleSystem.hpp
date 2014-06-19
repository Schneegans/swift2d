////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_CPU_PARTICLE_SYSTEM_HPP
#define SWIFT2D_CPU_PARTICLE_SYSTEM_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/properties.hpp>
#include <swift2d/textures/Texture.hpp>

#include <stack>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class ParticleSystemComponent;

// shared pointer type definition ----------------------------------------------
class CPUParticleSystem;
typedef std::shared_ptr<CPUParticleSystem>       CPUParticleSystemPtr;
typedef std::shared_ptr<const CPUParticleSystem> ConstCPUParticleSystemPtr;
typedef Property<CPUParticleSystemPtr>           CPUParticleSystemProperty;

// -----------------------------------------------------------------------------
class CPUParticleSystem {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  CPUParticleSystem(ParticleSystemComponent* parent);
  ~CPUParticleSystem();

  // Creates a new component and returns a shared pointer.
  static CPUParticleSystemPtr create(ParticleSystemComponent* parent) {
    return std::make_shared<CPUParticleSystem>(parent);
  }

  // ------------------------------------------------------------ public methods
  void update(double time);

  // Draws the CPUParticleSystem to the given context.
  void draw(RenderContext const& context) const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context) const;
  void spawn();
  void clear_all();

  ParticleSystemComponent* parent_;

  mutable std::stack<int>            empty_positions_;
  mutable std::vector<math::vec2>    positions_;
  mutable std::vector<math::vec2>    directions_;
  mutable std::vector<float>         ages_;
  mutable std::vector<float>         max_ages_;

  mutable oglplus::VertexArray*      particles_;
  mutable oglplus::Buffer*           pos_buf_;
  mutable oglplus::Buffer*           age_buf_;
};

}

#endif // SWIFT2D_CPU_PARTICLE_SYSTEM_HPP
