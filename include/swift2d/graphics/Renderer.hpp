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
#include <swift2d/graphics/RenderClient.hpp>
#include <vector>
#include <string>
#include <memory>

namespace swift {

// forward declares ------------------------------------------------------------
class Scene;
class Pipeline;

typedef std::shared_ptr<Pipeline>     PipelinePtr;
typedef std::shared_ptr<const Scene>  ConstScenePtr;

////////////////////////////////////////////////////////////////////////////////
// Manages the rendering on multiple contexts.                                //
// This class is used to provide a renderer frontend interface to the user.   //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Renderer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  typedef std::vector<std::unique_ptr<const Scene>> scene_vec_t;
  typedef scene_vec_t const const_scene_vec_t;

  Renderer(std::vector<PipelinePtr> const& pipelines);
  virtual ~Renderer();

  void queue_draw(std::vector<Scene const*> const& scenes);

  void stop();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  typedef RenderClient<std::shared_ptr<const_scene_vec_t>> render_client_t;

  std::vector<render_client_t*> render_clients_;
  FPSCounter                    application_fps_;
};

}

#endif  // SWIFT2D_RENDERER_HPP
