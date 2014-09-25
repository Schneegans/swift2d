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
#include <swift2d/scene/Scene.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL SceneManager : public Singleton<SceneManager> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  ScenePtr const& current()                const { return current_; }
  void            current(ScenePtr const& scene) { current_ = scene; }

  friend class Singleton<SceneManager>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  SceneManager() : current_(Scene::create()) {}
  ~SceneManager() {}

  ScenePtr current_;
};

}

#endif  // SWIFT2D_SCENE_MANAGER_HPP
