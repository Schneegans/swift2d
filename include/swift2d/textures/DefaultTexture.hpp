////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DEFAULT_TEXTURE_HPP
#define SWIFT2D_DEFAULT_TEXTURE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/graphics/RenderContext.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class DefaultTexture : public Singleton<DefaultTexture> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Binds the texture on the given context to the given location.
  void bind(RenderContext const& context, unsigned location) const;

  friend class Singleton<DefaultTexture>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  // ---------------------------------------------------- construction interface
  DefaultTexture();
  ~DefaultTexture();

  void upload_to(RenderContext const& context) const;

  mutable oglplus::Texture* texture_;
};

}

#endif // SWIFT2D_DEFAULT_TEXTURE_HPP
