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
  , VignetteColor(Color(0, 0, 0, 1))
  , VignetteSoftness(0.5)
  , VignetteCoverage(0.5)
  , DirtOpacity(0.5)
  , GlowOpacity(0.5)
  , ColorMapName("")
  , ColorGradingIntensity(0, 0) {

  Root->add(Camera);
}

////////////////////////////////////////////////////////////////////////////////

void Scene::update(double time) {
  ColorGradingIntensity.update(time);
  if (Root) {
    Root->update(time);
  }
}

////////////////////////////////////////////////////////////////////////////////

SerializedScenePtr Scene::serialize() const {

  auto scene(SerializedScene::create());

  scene->camera = Camera->create_copy();
  scene->vignette_color = VignetteColor().vec4();
  scene->vignette_softness = VignetteSoftness();
  scene->vignette_coverage = VignetteCoverage();
  scene->dirt_opacity = DirtOpacity();
  scene->glow_opacity = GlowOpacity();

  scene->color_map_name = ColorMapName();
  scene->color_grading_intensity = ColorGradingIntensity();

  if (Root->Enabled()) {
    Root->serialize(scene);
  }

  return scene;
}

////////////////////////////////////////////////////////////////////////////////

}
