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
#include <swift2d/properties.hpp>
#include <swift2d/textures/Texture.hpp>

#include <stack>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class ParticleSystemComponent;

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
  ParticleSystem(ParticleSystemComponent* parent);

  // Creates a new component and returns a shared pointer.
  static ParticleSystemPtr create(ParticleSystemComponent* parent) {
    return std::make_shared<ParticleSystem>(parent);
  }

  // ------------------------------------------------------------ public methods
  void update(double time);

  // Draws the ParticleSystem to the given context.
  void draw(RenderContext const& context) const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context) const;

  inline int current_tf() const { return ping_ ? 1 : 0; }
  inline int current_vb() const { return ping_ ? 0 : 1; }

  ParticleSystemComponent* parent_;

  mutable float particles_to_spawn_;
  mutable std::vector<oglplus::TransformFeedback> transform_feedbacks_;
  mutable std::vector<oglplus::Buffer>            particle_buffers_;
  mutable std::vector<oglplus::VertexArray>       particle_vaos_;

  mutable bool ping_;
  double       frame_time_;
  double       total_time_;
};

}

#endif // SWIFT2D_PARTICLE_SYSTEM_HPP
