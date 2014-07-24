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
  ~ParticleSystem();

  // Creates a new component and returns a shared pointer.
  static ParticleSystemPtr create(ParticleSystemComponent* parent) {
    return std::make_shared<ParticleSystem>(parent);
  }

  // ------------------------------------------------------------ public methods
  void update(double time);

  // Draws the ParticleSystem to the given context.
  void draw(RenderContext const& context) const;

  // int get_particle_count() const {
  //   return positions_.size() - empty_positions_.size();
  // }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context) const;
  // void spawn();
  // void clear_all();

  inline int current_tf() const { return ping_ ? 1 : 0; }
  inline int current_vb() const { return ping_ ? 0 : 1; }

  ParticleSystemComponent* parent_;

  // std::stack<int>            empty_positions_;
  // std::vector<math::vec2>    positions_;
  // std::vector<math::vec2>    directions_;
  // std::vector<float>         ages_;
  // std::vector<float>         max_ages_;
  // std::vector<float>         rots_;
  // std::vector<float>         rot_speeds_;

  float particles_to_spawn_;
  mutable std::vector<oglplus::TransformFeedback> transform_feedbacks_;
  mutable std::vector<oglplus::Buffer>            particle_buffers_;
  mutable std::vector<oglplus::VertexArray>       particle_vaos_;
  oglplus::DefaultTransformFeedback default_transform_feedback_;

  mutable bool ping_;

  // mutable oglplus::VertexArray*      particles_;
  // mutable oglplus::Buffer*           pos_buf_;
  // mutable oglplus::Buffer*           age_buf_;
  // mutable oglplus::Buffer*           rot_buf_;
};

}

#endif // SWIFT2D_PARTICLE_SYSTEM_HPP
