////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PARTICLE_SYSTEM_HPP
#define SWIFT2D_PARTICLE_SYSTEM_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/particles/ParticleEmitterComponent.hpp>

#include <unordered_map>
#include <unordered_set>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class ParticleSystem;
typedef std::shared_ptr<ParticleSystem>       ParticleSystemPtr;
typedef std::shared_ptr<const ParticleSystem> ConstParticleSystemPtr;
typedef Property<ParticleSystemPtr>           ParticleSystemProperty;

// -----------------------------------------------------------------------------
class ParticleSystem {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  ParticleSystem();

  // Creates a new component and returns a shared pointer.
  static ParticleSystemPtr create() {
    return std::make_shared<ParticleSystem>();
  }

  // ------------------------------------------------------------ public methods
  void update(
    std::unordered_set<ParticleEmitterComponentPtr> const& emitters,
    double time);

  void update_particles(
    std::unordered_set<ParticleEmitterComponentPtr> const& emitters,
    float mass, float linear_damping, float angular_damping,
    RenderContext const& context);

  void draw_particles(RenderContext const& context);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context);

  inline int current_tf() const { return ping_ ? 1 : 0; }
  inline int current_vb() const { return ping_ ? 0 : 1; }

  std::vector<oglplus::TransformFeedback> transform_feedbacks_;
  std::vector<oglplus::Buffer>            particle_buffers_;
  std::vector<oglplus::VertexArray>       particle_vaos_;

  std::unordered_map<ParticleEmitterComponentPtr, float>  particles_to_spawn_;
  bool   ping_;
  double frame_time_;
  double total_time_;
};

}

#endif // SWIFT2D_PARTICLE_SYSTEM_HPP
