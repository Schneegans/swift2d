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
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/properties.hpp>
#include <swift2d/materials/Shader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Stores geometry data. A mesh can be loaded from an Assimp mesh and the     //
// draw onto a context.                                                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class ParticleSystem: public Singleton<ParticleSystem> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void update(double time);

  // Draws the ParticleSystem to the given context.
  void draw(RenderContext const& context, math::mat3 const& object_transform) const;

  friend class Singleton<ParticleSystem>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  ParticleSystem();
  ~ParticleSystem();

  void upload_to(RenderContext const& context) const;

  mutable std::vector<math::vec2>   positions_;
  mutable std::vector<float>        ages_;
  mutable Shader                    shader_;

  mutable oglplus::VertexArray*      particles_;
  mutable oglplus::Buffer*           pos_buf_;
  mutable oglplus::Buffer*           age_buf_;
};

}

#endif // SWIFT2D_PARTICLE_SYSTEM_HPP
