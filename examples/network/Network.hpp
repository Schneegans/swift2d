////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_NETWORK_HPP
#define SWIFT2D_NETWORK_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>

#include "Peer.hpp"
#include "UpnpOpener.hpp"
#include "HttpConnection.hpp"

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Network : public Singleton<Network> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void connect(std::string const& game_ID);
  void update();

  friend class Singleton<Network>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  Network() {}
  ~Network() {}

  Peer            peer_;
  HttpConnection  http_;
  UpnpOpener      upnp_;
  std::string     game_ID_;
};

}

#endif  // SWIFT2D_NETWORK_HPP
