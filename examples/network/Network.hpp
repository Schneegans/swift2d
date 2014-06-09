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

namespace RakNet {
  class RakPeerInterface;
}

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Network : public Singleton<Network> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  enum Phase {
    CONNECTING_TO_SERVER,
    OPENING_UPNP,
    SEARCHING_FOR_OTHER_INSTANCES,
    CONNECTING_TO_HOST,
    STARTING_NEW_INSTANCE,
    HOSTING_INSTANCE,
    CONNECTING_TO_PEERS,
    PARTICIPATING
  };

  enum PacketID {
    REQUEST_JOIN
  };

  // Signal<RakNet::Packet*> on_packet_received;

  void connect(std::string const& game_ID);
  void update();

  friend class Singleton<Network>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void enter_phase(Phase phase);
  void upload_game();

  Network();
  ~Network() {}

  Peer            peer_;
  HttpConnection  http_;
  UpnpOpener      upnp_;

  Phase           phase_;
  std::string     game_ID_;

  Timer           update_timer_;
  uint64_t        host_guid_;
  std::string     nat_server_address_;
};

}

#endif  // SWIFT2D_NETWORK_HPP
