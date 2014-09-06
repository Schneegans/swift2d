////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/Scene.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Scene::Scene()
  : Root(SceneObject::create())
  , Camera(CameraComponent::create())
  , VignetteColor(Color(0, 0, 0))
  , VignetteSoftness(0.5)
  , VignetteCoverage(0.5) {

  Root->add(Camera);
}

////////////////////////////////////////////////////////////////////////////////

SerializedScenePtr Scene::serialize() const {

  auto scene(SerializedScene::create());

  scene->camera = Camera->create_copy();
  scene->vignette_color = VignetteColor();
  scene->vignette_softness = VignetteSoftness();
  scene->vignette_coverage = VignetteCoverage();

  if (Root->Enabled()) {
    Root->serialize(scene);
  }

  return scene;
}

////////////////////////////////////////////////////////////////////////////////

}
