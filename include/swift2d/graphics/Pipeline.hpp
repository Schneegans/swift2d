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

  Pipeline();

  static PipelinePtr create() {
    return std::make_shared<Pipeline>();
  }

  Float application_fps;
  Float rendering_fps;

  void set_output_window(WindowPtr const& window);

  void draw(ConstSerializedScenePtr const& scene);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
    WindowPtr window_;
    math::vec2i old_size_;

    Compositor* compositor_;
};

}

#endif  // SWIFT2D_PIPELINE_HPP
