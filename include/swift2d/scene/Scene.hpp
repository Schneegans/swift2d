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
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/events.hpp>
#include <swift2d/math.hpp>

#include <unordered_set>
#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Scene;
typedef std::shared_ptr<Scene>        ScenePtr;
typedef std::shared_ptr<const Scene>  ConstScenePtr;

// -----------------------------------------------------------------------------
class Scene {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  // the name of this scene
  String pName;

  // ----------------------------------------------------- contruction interface
  static ScenePtr create() {
    return std::make_shared<Scene>();
  }

  // ------------------------------------------------------------ public methods
  // adds a new object to the scene and returns a shared pointer
  SceneObjectPtr add_object() {
    auto object(SceneObject::create());
    objects_.insert(object);
    return object;
  }

  // adds an existing object to the scene and returns a shared pointer
  SceneObjectPtr const& add_object(SceneObjectPtr const& object) {
    objects_.insert(object);
    return object;
  }

  // removes a given object from this scene
  void remove_object(SceneObjectPtr const& object) {
    objects_.erase(object);
  }

  // calls update() on each contained object
  virtual void update();

  // serializes this scene for a given camera
  SerializedScenePtr serialize() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  // a collection of all objects in this scene
  std::unordered_set<SceneObjectPtr> objects_;
};

}

#endif  // SWIFT2D_SCENE_HPP
