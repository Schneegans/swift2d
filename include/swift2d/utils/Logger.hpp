////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LOGGER_HPP
#define SWIFT2D_LOGGER_HPP

#include <iosfwd>

namespace swift {

#define LOG_TRACE   Logger::trace_impl  (__FILE__, __LINE__)
#define LOG_DEBUG   Logger::debug_impl  (__FILE__, __LINE__)
#define LOG_MESSAGE Logger::message_impl(__FILE__, __LINE__)
#define LOG_WARNING Logger::warning_impl(__FILE__, __LINE__)
#define LOG_ERROR   Logger::error_impl  (__FILE__, __LINE__)

class Logger {

 public:

  static bool enable_trace;
  static bool enable_debug;
  static bool enable_message;
  static bool enable_warning;
  static bool enable_error;

  static std::ostream& trace_impl(const char* file, int line);
  static std::ostream& debug_impl(const char* file, int line);
  static std::ostream& message_impl(const char* file, int line);
  static std::ostream& warning_impl(const char* file, int line);
  static std::ostream& error_impl(const char* file, int line);
};

}

#endif  // SWIFT2D_LOGGER_HPP
