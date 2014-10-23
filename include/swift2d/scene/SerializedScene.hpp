////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SERIALIZED_SCENE_HPP
#define SWIFT2D_SERIALIZED_SCENE_HPP

#include <swift2d/utils/Color.hpp>

#include <memory>
#include <vector>
#include <map>

namespace swift {

// forward declares ------------------------------------------------------------
class GravitySourceComponent;
typedef std::shared_ptr<GravitySourceComponent> GravitySourceComponentPtr;

class CameraComponent;
typedef std::shared_ptr<CameraComponent> CameraComponentPtr;

class DirectionalLightComponent;
typedef std::shared_ptr<DirectionalLightComponent> DirectionalLightComponentPtr;

class RendererPool;

// shared pointer type definition ----------------------------------------------
class SerializedScene;
typedef std::shared_ptr<SerializedScene>       SerializedScenePtr;
typedef std::shared_ptr<const SerializedScene> ConstSerializedScenePtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL SerializedScene {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  // ----------------------------------------------------- contruction interface
  static SerializedScenePtr create() {
    return std::make_shared<SerializedScene>();
  }

  SerializedScene();
  ~SerializedScene();

  RendererPool& renderers() const { return *renderers_; }

  std::multimap<float, DirectionalLightComponentPtr>  sun_lights;

  CameraComponentPtr camera;

  math::vec4  vignette_color;
  float       vignette_softness;
  float       vignette_coverage;
  float       dirt_opacity;

  std::string color_map_name;
  float       color_grading_intensity;

  std::vector<GravitySourceComponentPtr> gravity_sources;

 private:
  RendererPool* renderers_;
};

}

#endif  // SWIFT2D_SERIALIZED_SCENE_HPP
