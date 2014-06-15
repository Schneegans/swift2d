////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_APPLICATION_HPP
#define SWIFT2D_APPLICATION_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Application {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Application(int argc, char** argv);
  virtual ~Application() {}

  std::string get_resource(std::string const& type, std::string const& file) const;

  void start();
  void stop();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  std::string executable_;
  boost::asio::signal_set signals_;
};

}

#endif  // SWIFT2D_APPLICATION_HPP
