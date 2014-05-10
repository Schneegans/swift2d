////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SCENE_HPP
#define SWIFT2D_SCENE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Node.hpp>
#include <swift2d/math/math.hpp>
#include <swift2d/events/Properties.hpp>

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>

namespace swift {

// forward declares ------------------------------------------------------------
class Frustum;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Scene;
typedef std::shared_ptr<Scene>        ScenePtr;
typedef std::shared_ptr<const Scene>  ConstScenePtr;

// -----------------------------------------------------------------------------
class Scene {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface

  static ScenePtr create() {
    return std::make_shared<Scene>();
  }

  // creates a deep copy
  Scene deep_copy() const;

  // the name of this scene
  String name;

  // adds a new node to the scene and returns a shared pointer
  NodePtr const& add_node(NodePtr const& node) {
    nodes_.insert(node);
    return node;
  }

  // removes a given node from this scene
  void remove_node(NodePtr const& node) {
    nodes_.erase(node);
  }

  // serializes this scene for a given camera
  SerializedScene serialize(Frustum const& frustum) const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  // a collection of all nodes in this scene
  std::unordered_set<NodePtr> nodes_;
};

}

#endif  // SWIFT2D_SCENE_HPP
