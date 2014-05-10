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
  // True, if this core should be processed in the serialization and traversal.
  SceneObjectProperty pParent;
  Mat3                pTransform;
  Mat3                pWorldTransform;

  // ----------------------------------------------------- contruction interface
  static SceneObjectPtr create() {
    return std::make_shared<SceneObject>();
  }

  // ------------------------------------------------------------ public methods
  // add an existing component to this object
  template<typename T>
  typename std::shared_ptr<T> add_component(std::shared_ptr<T> const& component, int index = -1) {

    if (component->pUser.get() != nullptr) {
      component->pUser.get()->remove_component(component);
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
  typename std::shared_ptr<T> add_component(int index = -1) {
    auto component = std::make_shared<T>();
    return add_component(component, index);
  }

  // remove a component from this object
  void remove_component(ComponentPtr const& component);

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
  bool has_component() const {
    return get_component<T>() != nullptr;
  }

  // calls serialize() on all enabled components
  virtual void serialize(SerializedScenePtr& scene) const;

  virtual void update();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  std::vector<ComponentPtr>   components_;

  SceneObject*                parent_;
  std::vector<SceneObjectPtr> children_;
};

}

#endif  // SWIFT2D_SCENE_OBJECT_HPP
