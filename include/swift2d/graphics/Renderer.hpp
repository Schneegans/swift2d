////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_RENDERER_HPP
#define SWIFT2D_RENDERER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/FPSCounter.hpp>
#include <swift2d/events/Ticker.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <string>
#include <memory>
#include <mutex>

namespace swift {

// forward declares ------------------------------------------------------------
class Pipeline;
typedef std::shared_ptr<Pipeline> PipelinePtr;

class SceneObject;
typedef std::shared_ptr<SceneObject> SceneObjectPtr;

////////////////////////////////////////////////////////////////////////////////
// Manages the rendering on multiple contexts.                                //
// This class is used to provide a renderer frontend interface to the user.   //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Renderer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Renderer(Pipeline& pipeline);
  virtual ~Renderer();

  void process(SceneObjectPtr const& scene,
               CameraComponentPtr const& camera);

  void stop();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  ConstSerializedScenePtr rendered_scene_;
  ConstSerializedScenePtr updating_scene_;
  ConstSerializedScenePtr updated_scene_;

  TickerPtr ticker_;

  boost::thread forever_;
  std::mutex mutex_;
  bool running_, started_rendering_;
};

}

#endif  // SWIFT2D_RENDERER_HPP
