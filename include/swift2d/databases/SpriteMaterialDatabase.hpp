////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_MATERIAL_DATABASE_HPP
#define SWIFT2D_SPRITE_MATERIAL_DATABASE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/databases/Database.hpp>
#include <swift2d/materials/SpriteMaterial.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SpriteMaterialDatabase : public Database<SpriteMaterial>,
                               public Singleton<SpriteMaterialDatabase> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  friend class Singleton<SpriteMaterialDatabase>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  SpriteMaterialDatabase() {}
  ~SpriteMaterialDatabase() {}

};

}

#endif  // SWIFT2D_SPRITE_MATERIAL_DATABASE_HPP
