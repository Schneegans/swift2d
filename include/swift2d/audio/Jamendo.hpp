////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_JAMENDO_HPP
#define SWIFT2D_JAMENDO_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/network/HttpConnection.hpp>

#include <vector>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// -----------------------------------------------------------------------------
class SWIFT_DLL Jamendo : public Singleton<Jamendo> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  struct Track {
    std::string title;
    std::string artist;
    std::string album;
    std::string url;
  };

  struct Album {
    std::vector<Track> tracks;
  };


  // ------------------------------------------------------------------- signals
  Signal<Album> on_album_loaded;

  // ------------------------------------------------------------ public methods

  void load_album(std::string const& id);
  void update();

  friend class Singleton<Jamendo>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Jamendo();
  ~Jamendo() {}

  HttpConnection  http_;
  Album           album_;
};

}

#endif  // SWIFT2D_JAMENDO_HPP
