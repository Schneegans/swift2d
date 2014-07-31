////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LIGHT_MATERIAL_DATABASE_HPP
#define SWIFT2D_LIGHT_MATERIAL_DATABASE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/databases/Database.hpp>
#include <swift2d/materials/LightMaterial.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class LightMaterialDatabase : public Database<LightMaterial>,
                              public Singleton<LightMaterialDatabase> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  friend class Singleton<LightMaterialDatabase>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  LightMaterialDatabase() {}
  ~LightMaterialDatabase() {}

};

}

#endif  // SWIFT2D_LIGHT_MATERIAL_DATABASE_HPP
