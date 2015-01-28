////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PATHS_HPP
#define SWIFT2D_PATHS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>

#include <set>
#include <string>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Paths : public Singleton<Paths> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  void init(int argc, char** argv);
  void clean_up();

  std::string tmp_file(std::string const& suffix = "tmp") const;
  std::string resource(std::string const& type, std::string const& file) const;
  std::string make_absolute(std::string const& file) const;
  std::string get_extension(std::string const& file) const;
  std::set<std::string> list_files(std::string const& directory) const;

  friend class Singleton<Paths>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Paths();
  ~Paths() {}

  std::string executable_;
};

}

#endif  // SWIFT2D_PATHS_HPP
