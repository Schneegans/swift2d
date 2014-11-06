////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TEXTURE_HPP
#define SWIFT2D_TEXTURE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/properties.hpp>

#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// shared pointer type definition ----------------------------------------------
class Texture;
typedef std::shared_ptr<Texture>       TexturePtr;
typedef std::shared_ptr<const Texture> ConstTexturePtr;
typedef Property<TexturePtr>           TextureProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL Texture : public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  String FileName;
  Bool AsyncLoading;

  // ---------------------------------------------------- construction interface
  template <typename... Args>
  static TexturePtr create(Args&& ... a) {
    return std::make_shared<Texture>(a...);
  }

  Texture();
  Texture(std::string const& file_name);

  virtual ~Texture();

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Texture"; }

  // Binds the texture on the given context to the given location.
  virtual void bind(RenderContext const& context, unsigned location) const;

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:
  virtual void upload_to(RenderContext const& context) const;

  void load_texture_data() const;
  void free_texture_data() const;

  mutable oglplus::Texture* texture_;

  mutable bool needs_update_, loading_;

  mutable int width_, height_, channels_;
  mutable unsigned char* data_;

};

}

#endif // SWIFT2D_TEXTURE_HPP
