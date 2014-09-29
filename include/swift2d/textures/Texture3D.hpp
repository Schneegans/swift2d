////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TEXTURE_3D_HPP
#define SWIFT2D_TEXTURE_3D_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// shared pointer type definition ----------------------------------------------
class Texture3D;
typedef std::shared_ptr<Texture3D>       Texture3DPtr;
typedef std::shared_ptr<const Texture3D> ConstTexture3DPtr;
typedef Property<Texture3DPtr>           Texture3DProperty;

// -----------------------------------------------------------------------------
class Texture3D : public Texture {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  UInt TilesX;
  UInt TilesY;

  // ---------------------------------------------------- construction interface
  template <typename... Args>
  static Texture3DPtr create(Args&& ... a) {
    return std::make_shared<Texture3D>(a...);
  }

  Texture3D();
  Texture3D(std::string const& file_name, unsigned tiles_x, unsigned tiles_y);

  Texture3D(Color const& fbl, Color const& ftl, Color const& ftr, Color const& fbr,
            Color const& bbl, Color const& btl, Color const& btr, Color const& bbr,
            int width, int height, int depth);

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Texture3D"; }

  // Binds the texture on the given context to the given location.
  virtual void bind(RenderContext const& context, unsigned location) const;

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context) const;
};

}

#endif // SWIFT2D_TEXTURE_3D_HPP