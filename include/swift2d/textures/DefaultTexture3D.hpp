////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DEFAULT_ANIMATED_TEXTURE_HPP
#define SWIFT2D_DEFAULT_ANIMATED_TEXTURE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>

#include <swift2d/opengl.hpp>
#include <swift2d/graphics/RenderContext.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL DefaultTexture3D : public Singleton<DefaultTexture3D> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Binds the texture to the given location.
  void bind(unsigned location) const;

  friend class Singleton<DefaultTexture3D>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  // ---------------------------------------------------- construction interface
  DefaultTexture3D();
  ~DefaultTexture3D();

  void upload() const;

  mutable oglplus::Texture* texture_;
};

}

#endif // SWIFT2D_DEFAULT_ANIMATED_TEXTURE_HPP
