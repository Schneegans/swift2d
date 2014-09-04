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
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/components/CameraComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SceneManager : public Singleton<SceneManager> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  SceneObjectPtr     const& current_scene()  const { return current_scene_; }
  CameraComponentPtr const& current_camera() const { return current_camera_; }

  void current_scene (SceneObjectPtr const& scene) { current_scene_ = scene; }
  void current_camera(CameraComponentPtr const& cam) { current_camera_ = cam; }

  friend class Singleton<SceneManager>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  SceneManager()
    : current_scene_(SceneObject::create())
    , current_camera_(CameraComponent::create()) {

    current_scene_->add(current_camera_);
  }

  ~SceneManager() {}

  SceneObjectPtr     current_scene_;
  CameraComponentPtr current_camera_;
};

}

#endif  // SWIFT2D_SCENE_MANAGER_HPP
