////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef FIBRGLASS_TEXTURE_HPP
#define FIBRGLASS_TEXTURE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/properties.hpp>

#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Stores geometry data. A mesh can be loaded from an Assimp mesh and the     //
// draw onto a context.                                                       //
////////////////////////////////////////////////////////////////////////////////


// shared pointer type definition ----------------------------------------------
class Texture;
typedef std::shared_ptr<Texture>       TexturePtr;
typedef std::shared_ptr<const Texture> ConstTexturePtr;
typedef Property<TexturePtr>           TextureProperty;

// -----------------------------------------------------------------------------
class Texture {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  template <typename... Args>
  static TexturePtr create(Args&& ... a) {
    return std::make_shared<Texture>(a...);
  }

  Texture() {};
  Texture(std::string const& file_name);
  ~Texture();

  // initializes the contained data from a given texture file.
  void load_from_file(std::string const& file_name);

  // Binds the texture on the given context to the given location.
  void bind(RenderContext const& context, unsigned location) const;
  void unbind() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context) const;

  mutable oglplus::Texture *texture_;

  mutable bool needs_update_;
  std::string  file_name_;

};

}

#endif // FIBRGLASS_TEXTURE_HPP
