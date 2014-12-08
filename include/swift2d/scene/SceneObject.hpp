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

  SceneObject();

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "SceneObject"; }

  // removes this scene object from its parent
  void detach();

  //----------------------------------------------------- scene object interface

  // adds a new object to the scene and returns a shared pointer
  SceneObjectPtr add_object();

  // adds an existing object to the scene and returns a shared pointer
  SceneObjectPtr const& add_object(SceneObjectPtr const& object);

  // adds an existing object to the top level node of the scene and
  // returns a shared pointer
  SceneObjectPtr const& add_at_root(SceneObjectPtr const& object);

  std::unordered_set<SceneObjectPtr> const& get_objects() const;

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

  // calls update() on all components and objects
  virtual void update(double time);

  virtual void update_world_transform();

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // static SceneObjectPtr create_from_json(boost::property_tree::ptree const& json);

  // a collection of all objects attached to this object
  std::unordered_set<SceneObjectPtr> objects_;

  // a collection of all components attached to this object
  std::vector<ComponentPtr> components_;

  bool remove_flag_;

};

}

#endif  // SWIFT2D_SCENE_OBJECT_HPP
