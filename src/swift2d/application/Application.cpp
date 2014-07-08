////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/application/Application.hpp>

#include <boost/filesystem.hpp>
#include <swift2d/swift2d.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Application::Application()
  : executable_()
  , signals_(MainLoop::instance()->get_io_service(), SIGINT, SIGTERM) {

  signals_.async_wait([&](boost::system::error_code const& error, int signal_number){
    if (!error) {
      stop();
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

void Application::init(int argc, char** argv) {
  swift::init(argc, argv);
  executable_ = boost::filesystem::system_complete(argv[0]).normalize().remove_filename().string();
}

////////////////////////////////////////////////////////////////////////////////

std::string Application::get_resource(std::string const& type, std::string const& file) const {
  return executable_ + "/" + type + "/" + file;
}

////////////////////////////////////////////////////////////////////////////////

std::string Application::make_absolute(std::string const& file) const {
  return executable_ + "/" + file;
}

////////////////////////////////////////////////////////////////////////////////

void Application::start() {
  MainLoop::instance()->start();
}

////////////////////////////////////////////////////////////////////////////////

void Application::stop() {
  MainLoop::instance()->stop();
}

////////////////////////////////////////////////////////////////////////////////

}
