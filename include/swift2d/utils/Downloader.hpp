////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DOWNLOADER_HPP
#define SWIFT2D_DOWNLOADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/properties.hpp>

#include <string>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Downloader {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Signal<std::string> on_error;

  Int32 ProgressBytes;
  Float ProgressPercent;

  Downloader();

  void download(std::string const& uri);
  void download(std::string const& uri, std::string const& result_file);

  void update();

  std::string const& result_file() const { return result_; }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  static size_t write_func(void *ptr, size_t size, size_t nmemb, FILE *stream, void* user_data);
  static int progress_func(void* s, long total, long now, long, long);

  std::string result_;
  int current_dl_, total_dl_;

  std::string error_;
};

}

#endif  // SWIFT2D_DOWNLOADER_HPP
