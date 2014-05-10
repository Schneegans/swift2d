////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/utils/Logger.hpp>

#include <sstream>
#include <boost/iostreams/stream.hpp>

#define PRINT_RED "\x001b[0;31m"
#define PRINT_GREEN "\x001b[0;32m"
#define PRINT_YELLOW "\x001b[0;33m"
#define PRINT_BLUE "\x001b[0;34m"
#define PRINT_PURPLE "\x001b[0;35m"
#define PRINT_TURQUOISE "\x001b[0;36m"

#define PRINT_RED_BOLD "\x001b[1;31m"
#define PRINT_GREEN_BOLD "\x001b[1;32m"
#define PRINT_YELLOW_BOLD "\x001b[1;33m"
#define PRINT_BLUE_BOLD "\x001b[1;34m"
#define PRINT_PURPLE_BOLD "\x001b[1;35m"
#define PRINT_TURQUOISE_BOLD "\x001b[1;36m"

#define PRINT_RESET "\x001b[0m"

namespace swift {

bool Logger::enable_debug = true;
bool Logger::enable_message = true;
bool Logger::enable_warning = true;
bool Logger::enable_error = true;

namespace {
  namespace io = boost::iostreams;
  io::null_sink                     null_sink;
  io::stream_buffer<io::null_sink>  null_buffer(null_sink);
  std::ostream                      dev_null(&null_buffer);

  std::string location_string(const char* f, int l) {
    std::string file(std::string(f).substr(std::string(f).find_last_of('/') + 1).c_str());
    std::stringstream sstr;
    sstr << "[" << file << ":" << l << "]";
    return sstr.str();
  }

  std::ostream& print(bool enable, std::string const& header,
                      std::string const& color,
                      const char* file, int line) {
    if (enable) {
      return std::cout << color << header
                       << location_string(file, line)
                       << PRINT_RESET << " ";
    } else {
      return dev_null;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& Logger::debug_impl(const char* file, int line) {
  return print(enable_debug, "[SWIFT2D][D]", PRINT_BLUE, file, line);
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& Logger::message_impl(const char* file, int line) {
  return print(enable_message, "[SWIFT2D][M]", PRINT_GREEN, file, line);
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& Logger::warning_impl(const char* file, int line) {
  return print(enable_warning, "[SWIFT2D][W]", PRINT_YELLOW, file, line);
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& Logger::error_impl(const char* file, int line) {
  return print(enable_error, "[SWIFT2D][E]", PRINT_RED, file, line);
}

////////////////////////////////////////////////////////////////////////////////

}
