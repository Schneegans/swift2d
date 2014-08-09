////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PIPELINE_HPP
#define SWIFT2D_PIPELINE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/events.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/graphics/Compositor.hpp>

#include <memory>
#include <vector>

namespace swift {

// forward declares ------------------------------------------------------------
class Window;
typedef std::shared_ptr<Window> WindowPtr;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Pipeline;
typedef std::shared_ptr<Pipeline>       PipelinePtr;
typedef std::shared_ptr<const Pipeline> ConstPipelinePtr;

// -----------------------------------------------------------------------------
class Pipeline {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  Int  ShadingQuality;
  Bool SuperSampling;

  // ---------------------------------------------------------------- properties
  Pipeline();
  ~Pipeline();

  static PipelinePtr create() {
    return std::make_shared<Pipeline>();
  }

  void set_output_window(WindowPtr const& window);

  void update();
  void draw(ConstSerializedScenePtr const& scene);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
    WindowPtr window_;
    math::vec2i old_size_;

    Compositor* compositor_;

    int max_load_amount_;
    int current_load_amount_;

    bool needs_reload_;
};

}

#endif  // SWIFT2D_PIPELINE_HPP
