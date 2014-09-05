////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SCENE_HPP
#define SWIFT2D_SCENE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/components/CameraComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Scene;
typedef std::shared_ptr<Scene>       ScenePtr;
typedef std::shared_ptr<const Scene> ConstScenePtr;

// -----------------------------------------------------------------------------
class Scene {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  SceneObjectPtr     Root;
  CameraComponentPtr Camera;

  // ----------------------------------------------------- contruction interface
  static ScenePtr create() {
    return std::make_shared<Scene>();
  }

  Scene()
    : Root(SceneObject::create())
    , Camera(CameraComponent::create()) {

    Root->add(Camera);
  }

};

}

#endif  // SWIFT2D_SCENE_HPP
