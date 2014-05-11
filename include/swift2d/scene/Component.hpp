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
#include <swift2d/events.hpp>
#include <swift2d/scene/SerializedScene.hpp>

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

typedef Property<SceneObject*>             SceneObjectProperty;

////////////////////////////////////////////////////////////////////////////////
// SceneObjects are made of multiple Components. A core encapsulates fine-grained         //
// functionality, data or behaviors. During scene traversal and serialization //
// each core may influence the result and may pass information to subsequent  //
// cores.                                                                     //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Component;
typedef std::shared_ptr<Component> ComponentPtr;

// -----------------------------------------------------------------------------
class Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  // True, if this core should be processed in the serialization and traversal.
  Bool                pEnabled;
  SceneObjectProperty pUser;


  // ----------------------------------------------------- contruction interface
  // Contstructor is protected --- do not instantiate this class directly
  virtual ~Component() {}

  // ------------------------------------------------------------ public methods
  virtual void update() {}

  virtual void serialize(SerializedScenePtr& scene) const {};

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:
  Component() : pEnabled(true), pUser(nullptr) {}

};

}

#endif  // SWIFT2D_COMPONENT_HPP
