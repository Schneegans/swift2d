////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/application/Application.hpp>

#include <swift2d/swift2d.hpp>
#include <swift2d/utils/Logger.hpp>

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Application::Application()
  : LoadingProgress(0.f)
  , AppFPS(20)
  , RenderFPS(20)
  , pipeline_()
  , renderer_()
  , window_()
  , signals_(MainLoop::get().get_io_service(), SIGINT, SIGTERM) {}

////////////////////////////////////////////////////////////////////////////////

void Application::init(int argc, char** argv) {

  swift::init();

  Paths::get().init(argc, argv);

  // init ctrl.c signal handler ------------------------------------------------
  signals_.async_wait([this](boost::system::error_code const& error,
                          int signal_number){
    if (!error) {
      stop();
    }
  });

  pipeline_ = Pipeline::create();
  renderer_ = Renderer::create(*pipeline_);
  window_   = WindowManager::get().get_default();
  pipeline_->set_output_window(window_);
}

////////////////////////////////////////////////////////////////////////////////

void Application::start() {
  AppFPS.start();
  RenderFPS.start();

  MainLoop::get().start();
}

////////////////////////////////////////////////////////////////////////////////

void Application::stop() {
  renderer_->stop();
  MainLoop::get().stop();
}

////////////////////////////////////////////////////////////////////////////////

void Application::display(SceneObjectPtr const& scene, CameraComponentPtr const& camera) {
  renderer_->process(scene, camera);
}

////////////////////////////////////////////////////////////////////////////////

void Application::clean_up() {
  Paths::get().clean_up();
  swift::clean_up();
}

////////////////////////////////////////////////////////////////////////////////

}
