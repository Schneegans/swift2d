////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SCENE_MANAGER_HPP
#define SWIFT2D_SCENE_MANAGER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SceneManager : public Singleton<SceneManager> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  SceneObjectPtr const& get_default() const { return default_; }

  friend class Singleton<SceneManager>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  SceneManager(): default_(SceneObject::create()) {}
  ~SceneManager() {}

  SceneObjectPtr default_;
};

}

#endif  // SWIFT2D_SCENE_MANAGER_HPP
