////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SAVABLE_OBJECT_HPP
#define SWIFT2D_SAVABLE_OBJECT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/objects/Object.hpp>
#include <swift2d/properties.hpp>

#include <memory>
#include <functional>
#include <unordered_map>

namespace swift {

// forward declares ------------------------------------------------------------
class SavableObjectVisitor;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SavableObject;
typedef std::shared_ptr<SavableObject>       SavableObjectPtr;
typedef std::shared_ptr<const SavableObject> ConstSavableObjectPtr;
typedef Property<SavableObjectPtr>           SavableObjectProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL SavableObject : public Object {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void save_to_file(std::string const& path);
  static SavableObjectPtr create_from_file(std::string const& path);

  virtual void accept(SavableObjectVisitor& to) {};
};

}

#endif  // SWIFT2D_SAVABLE_OBJECT_HPP
