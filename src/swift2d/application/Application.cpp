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

#include <boost/filesystem.hpp>
#include <boost/asio.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Application::Application()
  : LoadingProgress(1.f)
  , AppFPS(20)
  , RenderFPS(20)
  , signals_(new boost::asio::signal_set(MainLoop::get().get_io_service(), SIGINT, SIGTERM))
  , pipeline_()
  , renderer_() {}

////////////////////////////////////////////////////////////////////////////////

Application::~Application() {
  delete signals_;
}

////////////////////////////////////////////////////////////////////////////////

void Application::init(int argc, char** argv) {

  swift::init();

  Paths::get().init(argc, argv);

  // init ctrl.c signal handler ------------------------------------------------
  signals_->async_wait([this](boost::system::error_code const& error,
                            int signal_number){
    if (!error) {
      stop();
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

void Application::start() {
  pipeline_ = Pipeline::create();
  renderer_ = Renderer::create(*pipeline_);
  pipeline_->set_output_window(WindowManager::get().current());

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

void Application::clean_up() {
  signals_->cancel();
  Paths::get().clean_up();
  swift::clean_up();
}

////////////////////////////////////////////////////////////////////////////////

}
