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
class SWIFT_DLL Texture3D : public Texture {

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

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Texture3D"; }

  // Binds the texture on the given context to the given location.
  virtual void bind(RenderContext const& context, unsigned location) const;

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface

 protected:

  void set_width(int width) const;
  void set_height(int height) const;
  void set_channels(int channels) const;
  void set_data(unsigned char* data) const;

  virtual void upload_to(RenderContext const& context, bool create_mip_maps = true) const;

};

}

#endif // SWIFT2D_TEXTURE_3D_HPP
