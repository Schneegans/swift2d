////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_ANIMATED_TEXTURE_HPP
#define SWIFT2D_ANIMATED_TEXTURE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/textures/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// shared pointer type definition ----------------------------------------------
class AnimatedTexture;
typedef std::shared_ptr<AnimatedTexture>       AnimatedTexturePtr;
typedef std::shared_ptr<const AnimatedTexture> ConstAnimatedTexturePtr;
typedef Property<AnimatedTexturePtr>           AnimatedTextureProperty;

// -----------------------------------------------------------------------------
class AnimatedTexture : public Texture {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  UInt TilesX;
  UInt TilesY;

  // ---------------------------------------------------- construction interface
  template <typename... Args>
  static AnimatedTexturePtr create(Args&& ... a) {
    return std::make_shared<AnimatedTexture>(a...);
  }

  AnimatedTexture();
  AnimatedTexture(std::string const& file_name, unsigned tiles_x, unsigned tiles_y);

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "AnimatedTexture"; }

  // Binds the texture on the given context to the given location.
  virtual void bind(RenderContext const& context, unsigned location) const;

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context) const;
};

}

#endif // SWIFT2D_ANIMATED_TEXTURE_HPP
