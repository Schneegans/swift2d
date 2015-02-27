////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
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
  , Label("")
  , Depth(0)
  , WorldDepth(0)
  , Transform()
  , WorldTransform()
  , Enabled(true)
  , remove_flag_(false)
  , initialized_(false) {}

////////////////////////////////////////////////////////////////////////////////

SceneObject::SceneObject(SceneObject const& to_copy)
  : Parent(nullptr)
  , Label(to_copy.Label())
  , Depth(to_copy.Depth())
  , Transform(to_copy.Transform())
  , WorldTransform(to_copy.WorldTransform)
  , WorldDepth(to_copy.WorldDepth())
  , Enabled(to_copy.Enabled())
  , remove_flag_(to_copy.remove_flag_)
  , initialized_(to_copy.initialized_) {

  for (auto const& o:to_copy.objects_) {
    add_object(o->create_copy());
  }

  for (auto const& c:to_copy.components_) {
    add(c->create_base_copy());
  }
}

////////////////////////////////////////////////////////////////////////////////

SceneObjectPtr SceneObject::create_from_file(std::string const& path) {
  return std::dynamic_pointer_cast<SceneObject>(SavableObject::create_from_file(path));
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::detach(bool force) {
  if (Parent()) {
    Parent()->remove(this, force);
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::on_detach(double time) {
  for (auto current(components_.begin()), next(current);
       current != components_.end(); current = next) {
    ++next;
    (*current)->on_detach(time);
  }

  for (auto current(objects_.begin()), next(current);
       current != objects_.end(); current = next) {
    ++next;
    (*current)->on_detach(time);
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

SceneObjectPtr SceneObject::get_object(std::string const& label) const {
  if (label[0] == '/') {
    return get_root()->get_object(split_string(label.substr(1), '/'));
  }

  return get_object(split_string(label, '/'));
}

////////////////////////////////////////////////////////////////////////////////

SceneObjectPtr SceneObject::get_object(std::vector<std::string> const& path) const {
  return get_object(path.begin(), path.end());
}

////////////////////////////////////////////////////////////////////////////////

SceneObjectPtr SceneObject::get_object(
                          std::vector<std::string>::const_iterator const& path_start,
                          std::vector<std::string>::const_iterator const& path_end) const {

  if (path_start != path_end) {
    for (auto& ptr: objects_) {
      if (ptr->Label() == *path_start) {
        if (path_start+1 == path_end) {
          if (!ptr->remove_flag_) {
            return ptr;
          } else {
            return SceneObjectPtr();
          }
        } else {
          return get_object(path_start+1, path_end);
        }
      }
    }
  }

  return SceneObjectPtr();
}

////////////////////////////////////////////////////////////////////////////////

SceneObject const* SceneObject::get_root() const {
  if (Parent()) {
    return Parent()->get_root();
  } else {
    return this;
  }
}

////////////////////////////////////////////////////////////////////////////////

SceneObject* SceneObject::get_root() {
  if (Parent()) {
    return Parent()->get_root();
  } else {
    return this;
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::remove(SceneObjectPtr const& object, bool force) {
  if (force) {
    object->on_detach(0.0);
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
        object->on_detach(0.0);
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
    for (auto it(components_.begin()); it!=components_.end(); ++it) {
      if (*it == component) {
        component->on_detach(0.0);
        components_.erase(it);
        break;
      }
    }
  } else {
    component->remove_flag_ = true;
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::remove(Component* component, bool force) {
  if (force) {
    for (auto it(components_.begin()); it!=components_.end(); ++it) {
      if (it->get() == component) {
        component->on_detach(0.0);
        components_.erase(it);
        break;
      }
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

float SceneObject::get_rotation() const {
  return math::get_rotation(Transform.get());
}
float SceneObject::get_world_rotation() const {
  return math::get_rotation(WorldTransform.get());
}
math::vec2 SceneObject::get_direction() const {
  return (Transform.get() * math::vec3(1, 0, 0)).xy();
}
math::vec2 SceneObject::get_world_direction() const {
  return (WorldTransform.get() * math::vec3(1, 0, 0)).xy();
}
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

void SceneObject::on_init() {
  update_world_transform();

  {
    auto current(components_.begin());
    bool last(current == components_.end());

    while (!last) {
      auto next(current+1);
      last = (next == components_.end());

      if (!(*current)->initialized_) {
        (*current)->initialized_ = true;
        (*current)->on_init();
      }

      current = next;
    }
  }

  {
    auto current(objects_.begin());
    bool last(current == objects_.end());

    while (!last) {
      auto next = current;
      ++next;
      last = (next == objects_.end());

      if (!(*current)->initialized_) {
        (*current)->initialized_ = true;
        (*current)->on_init();
      }

      current = next;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::update(double time) {

  if (Enabled()) {
    update_world_transform();

    {
      auto current(components_.begin());
      bool last(current == components_.end());

      while (!last) {
        auto next(current+1);
        last = (next == components_.end());

        if (!(*current)->initialized_) {
          (*current)->initialized_ = true;
          (*current)->on_init();
        }
        if ((*current)->remove_flag_) {
          (*current)->on_detach(time);
          components_.erase(current);
        } else if ((*current)->Enabled.get()) {
          (*current)->update(time);
        }

        current = next;
      }
    }

    {
      auto current(objects_.begin());
      bool last(current == objects_.end());

      while (!last) {
        auto next = current;
        ++next;
        last = (next == objects_.end());

        if (!(*current)->initialized_) {
          (*current)->initialized_ = true;
          (*current)->on_init();
        }
        if ((*current)->remove_flag_) {
          (*current)->on_detach(time);
          objects_.erase(current);
        } else {
          (*current)->update(time);
        }

        current = next;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::update_world_transform() {

  if (Enabled()) {
    if (Parent.get()) {
      WorldTransform = Parent.get()->WorldTransform.get() * Transform.get();
      WorldDepth     = Parent.get()->WorldDepth.get() + Depth.get();
    } else {
      WorldTransform = Transform.get();
      WorldDepth     = Depth.get();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::update_world_transform_recursively() {

  if (Enabled()) {
    if (Parent.get()) {
      Parent.get()->update_world_transform();
      WorldTransform = Parent.get()->WorldTransform.get() * Transform.get();
      WorldDepth     = Parent.get()->WorldDepth.get() + Depth.get();
    } else {
      WorldTransform = Transform.get();
      WorldDepth     = Depth.get();
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
