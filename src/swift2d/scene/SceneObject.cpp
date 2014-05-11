////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/SceneObject.hpp>

#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/components/CameraComponent.hpp>

#include <iostream>
#include <algorithm>

namespace swift {


////////////////////////////////////////////////////////////////////////////////

SceneObjectPtr SceneObject::add() {
  auto object(SceneObject::create());
  objects_.insert(object);
  return object;
}

////////////////////////////////////////////////////////////////////////////////

SceneObjectPtr const& SceneObject::add(SceneObjectPtr const& object) {
  objects_.insert(object);
  return object;
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::remove(SceneObjectPtr const& object) {
  objects_.erase(object);
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::remove(ComponentPtr const& component) {
  auto delete_pos(std::remove(components_.begin(), components_.end(), component));

  if (delete_pos != components_.end()) {
    (*delete_pos)->set_user(nullptr);
    components_.erase(delete_pos, components_.end());
  }
}

////////////////////////////////////////////////////////////////////////////////

SerializedScenePtr SceneObject::serialize(CameraComponentPtr const& camera) const {

  auto scene(SerializedScene::create());
  scene->camera = camera->create_copy();
  serialize(scene);

  return scene;
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::serialize(SerializedScenePtr& scene) const {
  for (auto const& component: components_) {
    if (component->pEnabled.get()) {
      component->serialize(scene);
    }
  }

  for (auto const& object: objects_) {
    object->serialize(scene);
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::update(double time) {

  if (pParent.get()) {
    pWorldTransform = pTransform.get() * pParent.get()->pWorldTransform.get();
  } else {
    pWorldTransform = pTransform.get();
  }

  for (auto const& component: components_) {
    if (component->pEnabled.get()) {
      component->update(time);
    }
  }

  for (auto const& object: objects_) {
    object->update(time);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
