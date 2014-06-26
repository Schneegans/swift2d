////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_COMPONENT_HPP
#define SWIFT2D_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/properties.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/scene/SceneSaver.hpp>
#include <swift2d/utils/Object.hpp>
#include <swift2d/utils/Logger.hpp>

#include <vector>
#include <memory>

namespace swift {

// forward declares ------------------------------------------------------------
class SceneObject;
typedef std::shared_ptr<SceneObject>       SceneObjectPtr;
typedef std::shared_ptr<const SceneObject> ConstSceneObjectPtr;

class SerializedScene;
typedef std::shared_ptr<SerializedScene>       SerializedScenePtr;
typedef std::shared_ptr<const SerializedScene> ConstSerializedScenePtr;

typedef Property<SceneObject*>                 SceneObjectProperty;

////////////////////////////////////////////////////////////////////////////////
// SceneObjects are made of multiple Components. A component encapsulates     //
// fine-grained functionality, data or behaviors.                             //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Component;
typedef std::shared_ptr<Component> ComponentPtr;

// -----------------------------------------------------------------------------
class Component : public Object {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  // True, if this core should be processed in the serialization and traversal.
  Bool Enabled;

  // ----------------------------------------------------- contruction interface
  // Contstructor is protected --- do not instantiate this class directly
  virtual ~Component() {}

  // ------------------------------------------------------------ public methods
  virtual void update(double time) {}
  virtual void serialize(SerializedScenePtr& scene) const {};
  virtual void save(SceneSaver& saver);

  void         set_user(SceneObject* u) { user_ = u; }
  SceneObject* get_user() const { return user_; }

  friend class SceneObject;

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:
  Component();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  SceneObject* user_;

  bool remove_flag_;
};

}

#endif  // SWIFT2D_COMPONENT_HPP
