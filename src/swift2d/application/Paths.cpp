////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/application/Paths.hpp>

#include <swift2d/utils/Logger.hpp>

#include <boost/filesystem.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Paths::Paths()
  : executable_() {}

////////////////////////////////////////////////////////////////////////////////

void Paths::init(int argc, char** argv) {

  executable_ = boost::filesystem::system_complete(argv[0]).normalize().remove_filename().string();

  if (executable_ == "") {
    LOG_ERROR << "Failed to get executable path!" << std::endl;
    return;
  }

  // create tmp directory ------------------------------------------------------
  boost::filesystem::create_directory(executable_ + "/tmp");
}

////////////////////////////////////////////////////////////////////////////////

void Paths::clean_up() {

  // delete tmp directory ------------------------------------------------------
  if (!boost::filesystem::remove_all(executable_ + "/tmp")) {
    LOG_ERROR << "Failed to delete temporary directory!" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

std::string Paths::tmp_file(std::string const& suffix) const {
  std::wstring wpath(boost::filesystem::unique_path().native());
  std::string path(wpath.begin(), wpath.end());
  return executable_ + "/tmp/" + path + "." + suffix;
}

////////////////////////////////////////////////////////////////////////////////

std::string Paths::resource(std::string const& type, std::string const& file) const {
  return executable_ + "/resources/" + type + "/" + file;
}

////////////////////////////////////////////////////////////////////////////////

std::string Paths::make_absolute(std::string const& file) const {
  return executable_ + "/" + file;
}

////////////////////////////////////////////////////////////////////////////////

}
