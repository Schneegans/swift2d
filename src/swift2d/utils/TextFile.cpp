////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/utils/TextFile.hpp>

// guacamole headers
#include <swift2d/utils/Logger.hpp>

// external headers
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TextFile::TextFile()
  : file_name_("")
  , content_("")
  , is_loaded_(false) {}

////////////////////////////////////////////////////////////////////////////////

TextFile::TextFile(std::string const& file_name)
  : file_name_(file_name)
  , content_("")
  , is_loaded_(false) {}

////////////////////////////////////////////////////////////////////////////////

bool TextFile::is_valid() const {

  std::ifstream file(file_name_.c_str());

  if (file.fail()) {
    return false;
  }

  file.close();
  return true;
}

////////////////////////////////////////////////////////////////////////////////

std::string const& TextFile::get_content() const {

  if (is_loaded_) {
    return content_;
  }

  is_loaded_ = true;

  std::ifstream ifs(file_name_.c_str(), std::ifstream::in | std::ifstream::binary);
  if (!ifs) {
    LOG_WARNING << "Cannot open file \"" << file_name_ << "\"!" << std::endl;
    return content_;
  }

  std::stringstream oss;
  oss << ifs.rdbuf();

  if (!ifs && !ifs.eof()) {
    LOG_WARNING << "Error reading file \"" << file_name_ << "\"!" << std::endl;
    return content_;
  }

  content_ = std::string(oss.str());
  return content_;
}

////////////////////////////////////////////////////////////////////////////////

void TextFile::set_content(std::string const& content) {
  content_ = content;
  is_loaded_ = true;
}

////////////////////////////////////////////////////////////////////////////////

bool TextFile::save() const {

  if (!is_loaded_) {
    LOG_WARNING << "Unable to save file \"" << file_name_ << "\"! No content has been set." << std::endl;
    return false;
  }

  std::ofstream ofs(file_name_);
  if (!ofs) {
    LOG_WARNING << "Cannot open file \""<< file_name_ << "\"!" << std::endl;
    return false;
  }

  ofs << content_;
  ofs.close();

  return true;
}

////////////////////////////////////////////////////////////////////////////////

void TextFile::remove() {
  std::remove(file_name_.c_str());
  content_ = "";
  is_loaded_ = false;
}

////////////////////////////////////////////////////////////////////////////////

std::string const& TextFile::get_file_name() const {
  return file_name_;
}

////////////////////////////////////////////////////////////////////////////////

}
