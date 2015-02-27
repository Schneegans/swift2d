////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PARTICLE_SYSTEM_HPP
#define SWIFT2D_PARTICLE_SYSTEM_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/particles/ParticleSystemComponent.hpp>
#include <swift2d/events/Timer.hpp>
#include <swift2d/utils/Queue.hpp>

#include <unordered_map>
#include <unordered_set>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SerializedParticleSystemComponent;
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
  ParticleSystem(int max_count);
  ~ParticleSystem();

  // Creates a new component and returns a shared pointer.
  static ParticleSystemPtr create(int max_count) {
    return std::make_shared<ParticleSystem>(max_count);
  }

  // ------------------------------------------------------------ public methods
  void set_max_count(int max_count);

  void spawn(math::vec3 const& pos_rot, unsigned count = 1);
  void spawn(math::vec3 const& pos_rot, math::vec2 const& vel, unsigned count = 1);
  void spawn(std::vector<std::pair<math::vec3, math::vec2>> const& pos_rot_vel);

  int update_particles(
    ParticleSystemComponent::Serialized const& system,
    RenderContext const& context);

  void draw_particles(RenderContext const& context);

  int get_particle_count() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context);

  inline int current_tf() const { return ping_ ? 1 : 0; }
  inline int current_vb() const { return ping_ ? 0 : 1; }

  std::vector<ogl::TransformFeedback> transform_feedbacks_;
  std::vector<ogl::Buffer>            particle_buffers_;
  std::vector<ogl::VertexArray>       particle_vaos_;

  ogl::Query* query_;

  Queue<std::pair<math::vec3, math::vec2>> new_particles_;

  bool   ping_;
  int    update_max_count_;
  int    count_;
};

}

#endif // SWIFT2D_PARTICLE_SYSTEM_HPP
