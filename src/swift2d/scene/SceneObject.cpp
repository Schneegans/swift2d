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
#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/components/CameraComponent.hpp>
#include <swift2d/math/operators.hpp>

#include <iostream>
#include <algorithm>

namespace swift {


////////////////////////////////////////////////////////////////////////////////

SceneObjectPtr SceneObject::create_from_file(std::string const& path) {
  return std::dynamic_pointer_cast<SceneObject>(SavableObject::create_from_file(path));
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::detach() {
  if (Parent()) {
    Parent()->remove(this);
  }
}

////////////////////////////////////////////////////////////////////////////////

SceneObjectPtr SceneObject::add_object() {
  auto object(SceneObject::create());
  objects_.insert(object);
  object->Parent = this;
  return object;
}

////////////////////////////////////////////////////////////////////////////////

SceneObjectPtr const& SceneObject::add_object(SceneObjectPtr const& object) {
  objects_.insert(object);

  if (object->Parent()) {
    object->detach();
  }

  object->Parent = this;
  return object;
}

////////////////////////////////////////////////////////////////////////////////

SceneObjectPtr const& SceneObject::add_at_root(SceneObjectPtr const& object) {
  if (Parent()) {
    return Parent()->add_at_root(object);
  } else {
    return add_object(object);
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::remove(SceneObjectPtr const& object, bool force) {
  if (force) {
    objects_.erase(object);
  } else {
    object->remove_flag_ = true;
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::remove(SceneObject* object, bool force) {
  if (force) {
    for (auto& ptr: objects_) {
      if (ptr.get() == object) {
        objects_.erase(ptr);
        break;
      }
    }
  } else {
    object->remove_flag_ = true;
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::remove(ComponentPtr const& component, bool force) {
  if (force) {
    auto delete_pos(std::remove(components_.begin(), components_.end(), component));

    if (delete_pos != components_.end()) {
      (*delete_pos)->set_user(nullptr);
      components_.erase(delete_pos, components_.end());
    }
  } else {
    component->remove_flag_ = true;
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
    if (component->Enabled.get()) {
      component->serialize(scene);
    }
  }

  for (auto const& object: objects_) {
    object->serialize(scene);
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::update(double time) {

  if (Parent.get()) {
    WorldTransform = Transform.get() * Parent.get()->WorldTransform.get();
  } else {
    WorldTransform = Transform.get();
  }

  for (auto current(components_.begin()), next(current);
       current != components_.end(); current = next) {
    ++next;
    if ((*current)->remove_flag_) {
      components_.erase(current);
    } else if ((*current)->Enabled.get()) {
      (*current)->update(time);
    }
  }

  for (auto current(objects_.begin()), next(current);
       current != objects_.end(); current = next) {
    ++next;
    if ((*current)->remove_flag_) {
      objects_.erase(current);
    } else {
      (*current)->update(time);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("Transform", Transform);
  visitor.add_array("Components", components_);
  visitor.add_array("Objects", objects_);

  for (auto& ptr: objects_) {
    ptr->Parent = this;
  }

  for (auto& ptr: components_) {
    ptr->set_user(this);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
