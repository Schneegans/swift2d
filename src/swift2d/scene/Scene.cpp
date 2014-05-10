////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Scene.hpp>

#include <swift2d/scene/TraversalStateStack.hpp>
#include <swift2d/serializer/SerializedScene.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Scene Scene::deep_copy() const {

  Scene copy;
  copy.name.set(name.get());

  for (auto const& node: nodes_) {
    copy.add_node(node->create_copy());
  }

  for (auto const& node: nodes_) {
    node->clear_copy();
  }

  return copy;
}

////////////////////////////////////////////////////////////////////////////////

SerializedScene Scene::serialize(Frustum const& frustum) const {

  SerializedScene scene;
  // scene.frustum_ = frustum;

  TraversalStateStack state;

  for (auto const& node: nodes_) {
    node->serialize(state, scene);
  }

  return scene;
}

////////////////////////////////////////////////////////////////////////////////

}
