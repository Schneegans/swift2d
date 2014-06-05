////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PEER_HPP
#define SWIFT2D_PEER_HPP

// includes  -------------------------------------------------------------------
// #include <swift2d/events.hpp>

namespace RakNet {
  class RakPeerInterface;
  class ConnectionGraph2;
  class FullyConnectedMesh2;
}

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Peer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Peer();
  ~Peer();

  friend class UpnpOpener;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  RakNet::RakPeerInterface*    peer_;
  RakNet::ConnectionGraph2*    graph_;
  RakNet::FullyConnectedMesh2* mesh_;
};

}

#endif  // SWIFT2D_PEER_HPP
