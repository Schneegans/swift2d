////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PIPELINE_HPP
#define SWIFT2D_PIPELINE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/events.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/graphics/Compositor.hpp>
#include <swift2d/graphics/SubSampler.hpp>

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
class SWIFT_DLL Pipeline {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Pipeline();
  ~Pipeline();

  static PipelinePtr create() {
    return std::make_shared<Pipeline>();
  }

  void set_output_window(WindowPtr const& window);

  void update();
  void draw(ConstSerializedScenePtr const& scene);

  double get_total_time() const;
  double get_frame_time() const;

  SubSampler* get_sub_sampler(int level);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
    WindowPtr window_;
    math::vec2i old_size_;
    unsigned size_update_wait_;

    Compositor* compositor_;

    int max_load_amount_;
    int current_load_amount_;

    bool needs_reload_;

    std::vector<SubSampler*> sub_samplers_;
    Timer timer_;

    double frame_time_;
    double total_time_;
};

}

#endif  // SWIFT2D_PIPELINE_HPP
