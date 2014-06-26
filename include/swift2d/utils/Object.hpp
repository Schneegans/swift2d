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

#include <functional>
#include <unordered_map>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Object {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  static Object* create(std::string const& type_name);

  template<typename T>
  static void init() {
    factory_[T::get_type_name_static()] = [](){ return new T(); };
  }

  virtual std::string get_type_name() const = 0;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  static std::unordered_map<std::string, std::function<Object*()>> factory_;

};

}

#endif  // SWIFT2D_OBJECT_HPP
