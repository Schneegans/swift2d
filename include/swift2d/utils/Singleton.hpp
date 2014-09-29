////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SINGLETON_HPP
#define SWIFT2D_SINGLETON_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/platform.hpp>

#include <cstddef>

////////////////////////////////////////////////////////////////////////////////
// This is base class for singletons.                                         //
////////////////////////////////////////////////////////////////////////////////

namespace swift {

// -----------------------------------------------------------------------------
template <typename T>
class Singleton {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Gets the instance. Singletons are classes, which are only instanciated
  // once. This method will create this instance if necessary and return a
  // reference to it.
  static T& get() {
    if (instance_ == nullptr) {
      instance_ = new T;
    }

    return *instance_;
  }

  // Deletes the instance of this Singleton. The instance of this singleton will
  // be deleted by this method.
  static void destroy_instance() {
    if (instance_ != nullptr) {
      delete instance_;
      instance_ = nullptr;
    }
  }

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:

  // Constructor. Has to be private in derived classe.
  Singleton() {}

  // Destructor. Has to be private in derived classe.
  virtual ~Singleton() {}

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Singleton(Singleton const& copy) {}

  static T* instance_;
};

// the actual instance of this template
template <typename T> T* Singleton<T>::instance_ = nullptr;

}

#endif // SWIFT2D_SINGLETON_HPP
