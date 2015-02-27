////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TEXTURE_DATABASE_HPP
#define SWIFT2D_TEXTURE_DATABASE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/databases/Database.hpp>
#include <swift2d/textures/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL TextureDatabase : public Database<Texture>,
                                  public Singleton<TextureDatabase> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  TexturePtr lookup_or_load(std::string const& name) {
    if (name == "") {
      return TexturePtr();
    }

    if (has(name)) {
      return TextureDatabase::get().lookup(name);
    }

    auto tex = Texture::create(name);
    TextureDatabase::get().add(name, tex);

    return tex;
  }

  friend class Singleton<TextureDatabase>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  TextureDatabase() {}
  ~TextureDatabase() {}

};

}

#endif  // SWIFT2D_TEXTURE_DATABASE_HPP
