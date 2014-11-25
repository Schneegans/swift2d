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

SceneObject::SceneObject()
  : Parent(nullptr)
  , Enabled(true)
  , Depth(0)
  , WorldDepth(0)
  , remove_flag_(false) {}

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

std::unordered_set<SceneObjectPtr> const& SceneObject::get_objects() const {
  return objects_;
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

void SceneObject::scale(math::vec2 const& scale) {
  math::mat3 t(Transform());
  math::scale(t, scale);
  Transform = t;
}
void SceneObject::scale(float scale) {
  math::mat3 t(Transform());
  math::scale(t, scale);
  Transform = t;
}
void SceneObject::scale(float x, float y) {
  math::mat3 t(Transform());
  math::scale(t, x, y);
  Transform = t;
}
void SceneObject::rotate(float angle) {
  math::mat3 t(Transform());
  math::rotate(t, angle);
  Transform = t;
}
void SceneObject::translate(math::vec2 const& delta) {
  math::mat3 t(Transform());
  math::translate(t, delta);
  Transform = t;
}
void SceneObject::translate(float x, float y) {
  math::mat3 t(Transform());
  math::translate(t, x, y);
  Transform = t;
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 SceneObject::get_position() const {
  return math::get_translation(Transform.get());
}
math::vec2 SceneObject::get_world_position() const {
  return math::get_translation(WorldTransform.get());
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::serialize(SerializedScenePtr& scene) const {
  if (Enabled()) {
    for (auto const& component: components_) {
      if (component->Enabled.get()) {
        component->serialize(scene);
      }
    }

    for (auto const& object: objects_) {
      object->serialize(scene);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::update(double time) {

  if (Enabled()) {
    if (Parent.get()) {
      WorldTransform = Parent.get()->WorldTransform.get() * Transform.get();
      WorldDepth     = Parent.get()->WorldDepth.get() + Depth.get();
    } else {
      WorldTransform = Transform.get();
      WorldDepth     = Depth.get();
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
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::update_world_transform() {

  if (Enabled()) {
    if (Parent.get()) {
      Parent.get()->update_world_transform();
      WorldTransform = Parent.get()->WorldTransform.get() * Transform.get();
    } else {
      WorldTransform = Transform.get();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("Transform", Transform);
  visitor.add_member("Depth",     Depth);
  visitor.add_member("Label",     Label);
  visitor.add_member("Enabled",   Enabled);
  visitor.add_ptr_array("Components", components_);
  visitor.add_ptr_array("Objects",    objects_);

  for (auto& ptr: objects_) {
    ptr->Parent = this;
  }

  for (auto& ptr: components_) {
    ptr->set_user(this);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
