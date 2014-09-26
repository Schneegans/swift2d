////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_OBJECT_HPP
#define SWIFT2D_OBJECT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Logger.hpp>
#include <swift2d/utils/platform.hpp>

#include <memory>
#include <functional>
#include <unordered_map>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Object;
typedef std::shared_ptr<Object>       ObjectPtr;
typedef std::shared_ptr<const Object> ConstObjectPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL Object {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

   static ObjectPtr create(std::string const& type_name);

  template<typename T>
  static void init() {
    factory_[T::get_type_name_static()] = [](){ return std::make_shared<T>(); };
  }

  virtual std::string get_type_name() const = 0;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
   static std::unordered_map<std::string, std::function<ObjectPtr()>> factory_;

};

}

#endif  // SWIFT2D_OBJECT_HPP
