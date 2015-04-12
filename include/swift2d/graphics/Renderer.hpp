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

#ifndef SWIFT2D_RENDERER_HPP
#define SWIFT2D_RENDERER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/FPSCounter.hpp>
#include <swift2d/events/Ticker.hpp>
#include <swift2d/events/Timer.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace swift {

// forward declares ------------------------------------------------------------
class Pipeline;
typedef std::shared_ptr<Pipeline> PipelinePtr;

class SceneObject;
typedef std::shared_ptr<SceneObject> SceneObjectPtr;

class Renderer;
typedef std::shared_ptr<Renderer> RendererPtr;

////////////////////////////////////////////////////////////////////////////////
// Manages the rendering on multiple contexts.                                //
// This class is used to provide a renderer frontend interface to the user.   //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Renderer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Renderer(Pipeline& pipeline);
  virtual ~Renderer();

  static RendererPtr create(Pipeline& pipeline) {
    return std::make_shared<Renderer>(pipeline);
  }

  void process(SceneObjectPtr const& scene,
               CameraComponentPtr const& camera);

  void stop();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void render_step();
  void application_step();

  Pipeline& pipeline_;

  ConstSerializedScenePtr rendered_scene_;
  ConstSerializedScenePtr last_rendered_scene_;
  ConstSerializedScenePtr updating_scene_;
  ConstSerializedScenePtr updated_scene_;

  Scheduler scheduler_;
  Timer     timer_;

  bool running_, application_finished_;
  boost::thread forever_;
  std::mutex              copy_mutex_;
  std::condition_variable copy_available_;
};

}

#endif  // SWIFT2D_RENDERER_HPP
