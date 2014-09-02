////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_APPLICATION_HPP
#define SWIFT2D_APPLICATION_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/properties.hpp>
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/utils/FPSCounter.hpp>
#include <swift2d/graphics/Pipeline.hpp>
#include <swift2d/graphics/Renderer.hpp>

#include <boost/asio.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Application : public Singleton<Application> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------- signals
  Signal<> on_frame;

  // ---------------------------------------------------------------- properties
  Float      LoadingProgress;
  FPSCounter AppFPS;
  FPSCounter RenderFPS;

  // ------------------------------------------------------------ public methods
  void init(int argc, char** argv);
  void start();
  void stop();
  void clean_up();

  void display(SceneObjectPtr const& scene, CameraComponentPtr const& camera);

  friend class Singleton<Application>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Application();
  ~Application() {}

  PipelinePtr pipeline_;
  RendererPtr renderer_;
  WindowPtr   window_;

  boost::asio::signal_set signals_;
};

}

#endif  // SWIFT2D_APPLICATION_HPP