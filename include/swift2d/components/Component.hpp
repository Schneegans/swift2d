////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_COMPONENT_HPP
#define SWIFT2D_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/properties.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/components/SerializedComponent.hpp>
#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/objects/SavableObject.hpp>
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
class SWIFT_DLL Component : public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  // True, if this component should be processed in the serialization and
  // traversal
  Bool Enabled;
  String Label;

  // ----------------------------------------------------- contruction interface
  // Contstructor is protected --- do not instantiate this class directly
  virtual ~Component() {}

  virtual ComponentPtr create_base_copy() const = 0;

  // ------------------------------------------------------------ public methods
  // removes this component from its user - if not forced it will be
  // removed in the next frame
  void detach(bool force = false);

  // called when this component or it's user got removed from the scene
  // it's called instead of update, so the frame time is passed to this method
  virtual void on_detach(double time) {}

  // called before the very first update. this base class implementation does
  // nothing, so you do not have to call it from your implementation
  virtual void on_init() {}

  // called by the component's user when its own update is called. Usually this
  // happens once each frame its usually a bad idea to call this method directly
  virtual void update(double time) {}

  // returns the SceneObject the component is currently attached to
  SceneObject* get_user() const { return user_; }

  // gets the root object of the current user of this component
  SceneObject const* get_root() const;
  SceneObject*       get_root();

  virtual void serialize(SerializedScenePtr& scene) const {};
  virtual void accept(SavableObjectVisitor& visitor);

  friend class SceneObject;

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:
  Component();
  Component(Component const& to_copy);

  // called automatically when the component is attached to a SceneObject
  void set_user(SceneObject* u) { user_ = u; }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  SceneObject* user_;

  bool remove_flag_, initialized_;
};

}

#endif  // SWIFT2D_COMPONENT_HPP
