////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SERIALIZED_SCENE_HPP
#define SWIFT2D_SERIALIZED_SCENE_HPP

#include <memory>
#include <map>

namespace swift {

// forward declares ------------------------------------------------------------
class DrawableComponent;
typedef std::shared_ptr<DrawableComponent> DrawableComponentPtr;

class CameraComponent;
typedef std::shared_ptr<CameraComponent> CameraComponentPtr;

// shared pointer type definition ----------------------------------------------
class SerializedScene;
typedef std::shared_ptr<SerializedScene>       SerializedScenePtr;
typedef std::shared_ptr<const SerializedScene> ConstSerializedScenePtr;

// -----------------------------------------------------------------------------
class SerializedScene {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  // ----------------------------------------------------- contruction interface
  static SerializedScenePtr create() {
    return std::make_shared<SerializedScene>();
  }

  CameraComponentPtr camera;
  std::map<float, DrawableComponentPtr> objects;

};

}

#endif  // SWIFT2D_SERIALIZED_SCENE_HPP
