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
class Application : public Singleton<Application> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void init(int argc, char** argv);

  void start();
  void stop();

  std::string get_resource(std::string const& type, std::string const& file) const;
  std::string make_absolute(std::string const& file) const;

  friend class Singleton<Application>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Application();
  ~Application() {}

  std::string executable_;
  boost::asio::signal_set signals_;
};

}

#endif  // SWIFT2D_APPLICATION_HPP
