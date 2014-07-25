////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_NOISE_TEXTURE_HPP
#define SWIFT2D_NOISE_TEXTURE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/graphics/RenderContext.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class NoiseTexture : public Singleton<NoiseTexture> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Binds the texture on the given context to the given location.
  void bind(RenderContext const& context, unsigned location) const;

  friend class Singleton<NoiseTexture>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  // ---------------------------------------------------- construction interface
  NoiseTexture();
  ~NoiseTexture();

  void upload_to(RenderContext const& context) const;

  mutable oglplus::Texture* texture_;
};

}

#endif // SWIFT2D_NOISE_TEXTURE_HPP
