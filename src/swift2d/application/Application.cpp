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

Application::Application(int argc, char** argv)
  : loop_()
  , executable_(boost::filesystem::system_complete(argv[0]).normalize().remove_filename().string()) {

  init(argc, argv);
}

////////////////////////////////////////////////////////////////////////////////

Application::~Application() {

}

////////////////////////////////////////////////////////////////////////////////

std::string Application::get_resource(std::string const& type, std::string const& file) {
  return executable_ + "/" + type + "/" + file;
}

////////////////////////////////////////////////////////////////////////////////

void Application::start() {
  loop_.start();
}

////////////////////////////////////////////////////////////////////////////////

void Application::stop() {
  loop_.stop();
}

////////////////////////////////////////////////////////////////////////////////

}
