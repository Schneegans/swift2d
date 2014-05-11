////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SCENE_OBJECT_HPP
#define SWIFT2D_SCENE_OBJECT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Component.hpp>

#include <unordered_set>
#include <vector>
#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SceneObject;
typedef std::shared_ptr<SceneObject>       SceneObjectPtr;
typedef std::shared_ptr<const SceneObject> ConstSceneObjectPtr;

typedef Property<SceneObject*>             SceneObjectProperty;

// -----------------------------------------------------------------------------
class SceneObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  String              pName;
  SceneObjectProperty pParent;
  Mat3                pTransform;
  Mat3                pWorldTransform;

  // ----------------------------------------------------- contruction interface
  static SceneObjectPtr create() {
    return std::make_shared<SceneObject>();
  }

  // ------------------------------------------------------------ public methods


  //----------------------------------------------------- scene object interface
  // adds a new object to the scene and returns a shared pointer
  SceneObjectPtr add() {
    auto object(SceneObject::create());
    objects_.insert(object);
    return object;
  }

  // removes a given object from this scene
  void remove(SceneObjectPtr const& object) {
    objects_.erase(object);
  }

  // adds an existing object to the scene and returns a shared pointer
  SceneObjectPtr const& add(SceneObjectPtr const& object) {
    objects_.insert(object);
    return object;
  }

  //-------------------------------------------------------- component interface
  // add an existing component to this object
  template<typename T>
  typename std::shared_ptr<T> add(std::shared_ptr<T> const& component, int index = -1) {

    if (component->pUser.get() != nullptr) {
      component->pUser.get()->remove(component);
    }

    component->pUser = this;

    if (index < 0 || index >= components_.size()) {
      components_.push_back(component);
    } else {
      components_.insert(components_.begin() + index, component);
    }

    return component;
  }

  // create a new component for this object
  template<typename T>
  typename std::shared_ptr<T> add(int index = -1) {
    auto component = std::make_shared<T>();
    return add(component, index);
  }

  // remove a component from this object
  void remove(ComponentPtr const& component);

  // get all components of the given type
  template<typename T>
  std::vector<std::shared_ptr<T>> get_components() const {
    std::vector<std::shared_ptr<T>> result;

    for (auto const& component: components_) {
      auto casted(std::dynamic_pointer_cast<T>(component));
      if (casted) {
        result.push_back(casted);
      }
    }
    return result;
  }

  // get the first component of the given type
  template<typename T>
  std::shared_ptr<T> get_component() const {
    for (auto const& component: components_) {
      auto casted(std::dynamic_pointer_cast<T>(component));
      if (casted) {
        return casted;
      }
    }
    return std::shared_ptr<T>();
  }

  // returns true, if a component of the given type exists in this object
  template<typename T>
  bool has() const {
    return get_component<T>() != nullptr;
  }

  // calls serialize() on all enabled components
  virtual void serialize(SerializedScenePtr& scene) const;

  virtual SerializedScenePtr serialize() const;

  virtual void update();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  // a collection of all objects attached to this object
  std::unordered_set<SceneObjectPtr> objects_;

  // a collection of all components attached to this object
  std::vector<ComponentPtr> components_;

};

}

#endif  // SWIFT2D_SCENE_OBJECT_HPP
