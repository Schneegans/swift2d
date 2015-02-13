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
#include <swift2d/components/Component.hpp>
#include <swift2d/objects/SavableObject.hpp>

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
class SWIFT_DLL SceneObject : public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  SceneObjectProperty Parent;
  String              Label;

  Float               Depth;
  Float               WorldDepth;

  Mat3                Transform;
  Mat3                WorldTransform;
  Bool                Enabled;

  // ----------------------------------------------------- contruction interface
  static SceneObjectPtr create() {
    return std::make_shared<SceneObject>();
  }

  static SceneObjectPtr create_from_file(std::string const& path);

  SceneObjectPtr create_copy() {
    return std::make_shared<SceneObject>(*this);
  }

  SceneObject();
  SceneObject(SceneObject const& to_copy);

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "SceneObject"; }

  // removes this scene object from its parent - if not forced it will be
  // removed in the next frame
  void detach(bool force = false);

  // called when this object or one of it's parents got removed from the scene
  // it's called instead of update, so the frame time is passed to this method
  virtual void on_detach(double time);

  // called before the very first update. This base class implementation
  // updates the WorldTransform and calls on_init recursively on all attached
  // Components and SceneObjects. Be sure to extend this method only!
  virtual void on_init();

  // calls update() on all components and objects
  virtual void update(double time);

  virtual void update_world_transform();
  virtual void update_world_transform_recursively();

  //----------------------------------------------------- scene object interface

  // adds a new object to the scene and returns a shared pointer
  SceneObjectPtr add_object();

  // adds an existing object to the scene and returns a shared pointer
  SceneObjectPtr const& add_object(SceneObjectPtr const& object);

  // adds an existing object to the top level node of the scene and
  // returns a shared pointer
  SceneObjectPtr const& add_at_root(SceneObjectPtr const& object);

  // gets all children of this SceneObject
  std::unordered_set<SceneObjectPtr> const& get_objects() const;

  // gets the first child with the given label
  SceneObjectPtr      get_object(std::string const& label) const;
  SceneObjectPtr      get_object(std::vector<std::string> const& path) const;

  // gets the root object
  SceneObject const*  get_root() const;
  SceneObject*        get_root();

  // removes a given object from this scene
  void remove(SceneObjectPtr const& object, bool force = false);
  void remove(SceneObject* object, bool force = false);


  //-------------------------------------------------------- component interface

  // add an existing component to this object
  template<typename T>
  typename std::shared_ptr<T> add(std::shared_ptr<T> const& component, int index = -1) {

    if (component->get_user() != nullptr) {
      component->get_user()->remove(component);
    }

    component->set_user(this);

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
  void remove(ComponentPtr const& component, bool force = false);
  void remove(Component* component, bool force = false);

  // get all components
  std::vector<ComponentPtr> const& get_all_components() const {
    return components_;
  }

  // get all components of the given type
  template<typename T>
  std::vector<std::shared_ptr<T>> get_components() const {
    std::vector<std::shared_ptr<T>> result;

    for (auto const& component: components_) {
      auto casted(std::dynamic_pointer_cast<T>(component));
      if (casted && !casted->remove_flag_) {
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
      if (casted && !casted->remove_flag_) {
        return casted;
      }
    }
    return std::shared_ptr<T>();
  }

  // get the first component of the given type with the given label
  template<typename T>
  std::shared_ptr<T> get_component(std::string const& label) const {

    if (label[0] == '/') {
      return get_root()->get_component<T>(split_string(label.substr(1), '/'));
    }

    return get_component<T>(split_string(label, '/'));

    for (auto const& component: components_) {
      if (component->Label() == label) {
        auto casted(std::dynamic_pointer_cast<T>(component));
        if (casted && !casted->remove_flag_) {
          return casted;
        }
      }
    }
    return std::shared_ptr<T>();
  }

  // get the first component of the given type with the given path
  template<typename T>
  std::shared_ptr<T> get_component(std::vector<std::string> const& path) const {
    if (path.size() == 1) {
      for (auto const& component: components_) {
        if (component->Label() == path[0]) {
          auto casted(std::dynamic_pointer_cast<T>(component));
          if (casted && !casted->remove_flag_) {
            return casted;
          }
        }
      }
    } else if (path.size() > 1) {
      auto object(get_object(path.begin(), path.end()-1));
      if (object) {
        for (auto const& component: object->components_) {
          if (component->Label() == path.back()) {
            auto casted(std::dynamic_pointer_cast<T>(component));
            if (casted && !casted->remove_flag_) {
              return casted;
            }
          }
        }
      }
    }

    return std::shared_ptr<T>();
  }

  // returns true, if a component of the given type exists in this object
  template<typename T>
  bool has() const {
    return get_component<T>() != nullptr;
  }

  // -------------------------------------------------- transformation interface
  void scale     (math::vec2 const& scale);
  void scale     (float scale);
  void scale     (float x, float y);
  void rotate    (float angle);
  void translate (math::vec2 const& delta);
  void translate (float x, float y);

  virtual float      get_rotation() const;
  virtual float      get_world_rotation() const;
  virtual math::vec2 get_direction() const;
  virtual math::vec2 get_world_direction() const;
  virtual math::vec2 get_position() const;
  virtual math::vec2 get_world_position() const;

  // --------------------------------------------------- serialization interface

  // calls serialize() on all enabled components and objects --- the provided
  // SerializedScene is extended
  virtual void serialize(SerializedScenePtr& scene) const;

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  SceneObjectPtr get_object(std::vector<std::string>::const_iterator const& path_start,
                            std::vector<std::string>::const_iterator const& path_end) const;

  // a collection of all objects attached to this object
  std::unordered_set<SceneObjectPtr> objects_;

  // a collection of all components attached to this object
  std::vector<ComponentPtr> components_;

  bool remove_flag_, initialized_;
};

}

#endif  // SWIFT2D_SCENE_OBJECT_HPP
