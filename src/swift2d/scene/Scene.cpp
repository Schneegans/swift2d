////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Scene.hpp>


namespace swift {

////////////////////////////////////////////////////////////////////////////////

void Scene::update() {
  for (auto& object: objects_) {
    object->update();
  }
}

////////////////////////////////////////////////////////////////////////////////

SerializedScenePtr Scene::serialize() const {

  auto scene(SerializedScene::create());

  for (auto& object: objects_) {
    object->serialize(scene);
  }

  return scene;
}

////////////////////////////////////////////////////////////////////////////////

}
