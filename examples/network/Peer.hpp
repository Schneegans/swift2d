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
#include <string>

namespace RakNet {
  class RakPeerInterface;
  class ConnectionGraph2;
  class FullyConnectedMesh2;
  class NatPunchthroughClient;
  class NatTypeDetectionClient;
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

  void connect(std::string const& ip, unsigned short port);
  void open_nat(uint64_t guid, std::string const& nat_server);

  uint64_t get_guid() const;

  friend class UpnpOpener;
  friend class Network;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void request_join(uint64_t guid);
  void start_join(uint64_t guid);
  void join(uint64_t guid, std::string const& nat_server);

  RakNet::RakPeerInterface*       peer_;
  RakNet::ConnectionGraph2*       graph_;
  RakNet::FullyConnectedMesh2*    mesh_;
  RakNet::NatPunchthroughClient*  npt_;
  RakNet::NatTypeDetectionClient* nat_type_detector_;
};

}

#endif  // SWIFT2D_PEER_HPP
