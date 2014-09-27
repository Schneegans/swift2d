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

  LOG_WARNING << "trying to remove " << executable_ + "/tmp" << std::endl;
  // delete tmp directory ------------------------------------------------------
  //if (!boost::filesystem::remove_all(executable_ + "/tmp")) {
  //  LOG_ERROR << "Failed to delete temporary directory!" << std::endl;
 // }
}

////////////////////////////////////////////////////////////////////////////////

std::string Paths::tmp_file(std::string const& suffix) const {
  std::string file(boost::filesystem::unique_path().string());
  boost::filesystem::path p(executable_ + "/tmp/" + file + "." + suffix);
  return p.normalize().string();
}

////////////////////////////////////////////////////////////////////////////////

std::string Paths::resource(std::string const& type, std::string const& file) const {
  boost::filesystem::path p(executable_ + "/resources/" + type + "/" + file);
  return p.normalize().string();
}

////////////////////////////////////////////////////////////////////////////////

std::string Paths::make_absolute(std::string const& file) const {
  auto p(boost::filesystem::absolute(file, executable_));
  return p.normalize().string();
}

////////////////////////////////////////////////////////////////////////////////

}
