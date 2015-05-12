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

#ifndef SWIFT2D_LOGGER_HPP
#define SWIFT2D_LOGGER_HPP

#include <swift2d/utils/platform.hpp>
#include <iosfwd>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Prints beautiful messages to the console output.                           //
////////////////////////////////////////////////////////////////////////////////

class SWIFT_DLL Logger {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // If any of these is set to false, the corresponding messages are discarded.
  static bool enable_trace;
  static bool enable_debug;
  static bool enable_message;
  static bool enable_warning;
  static bool enable_error;

  static std::ostream& trace_impl  (const char* file, int line);
  static std::ostream& debug_impl  (const char* file, int line);
  static std::ostream& message_impl(const char* file, int line);
  static std::ostream& warning_impl(const char* file, int line);
  static std::ostream& error_impl  (const char* file, int line);
};

// Use these macros in your code like this:
//   LOG_MESSAGE << "hello world" << std::endl;
#define LOG_TRACE   Logger::trace_impl  (__FILE__, __LINE__)
#define LOG_DEBUG   Logger::debug_impl  (__FILE__, __LINE__)
#define LOG_MESSAGE Logger::message_impl(__FILE__, __LINE__)
#define LOG_WARNING Logger::warning_impl(__FILE__, __LINE__)
#define LOG_ERROR   Logger::error_impl  (__FILE__, __LINE__)

}

#endif  // SWIFT2D_LOGGER_HPP
