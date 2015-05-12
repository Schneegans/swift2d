////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DOWNLOADER_HPP
#define SWIFT2D_DOWNLOADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/properties.hpp>

#include <string>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Downloads binary data to file on disk.  Downloading happens in an extra    //
// thread.                                                                    //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Downloader {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------- signals
  Signal<std::string> on_error;

  // ---------------------------------------------------------------- properties
  Int32 ProgressBytes;
  Float ProgressPercent;

  // ----------------------------------------------------- contruction interface
  Downloader();

  // ------------------------------------------------------------ public methods

  // Downloads the resource at uri to a temporary file. Access the resulting
  // file name with result_file()
  void download(std::string const& uri);

  // Downloads the resource at uri to the given file.
  void download(std::string const& uri, std::string const& result_file);

  // Call this method regularily from your main thread. It will updated the
  // progress properties and emit on_error if an error occured.
  void update();

  // Returns the file name to which a file is downloaded.
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
