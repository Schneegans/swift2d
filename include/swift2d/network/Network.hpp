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

#include <swift2d/network/Peer.hpp>
#include <swift2d/network/UpnpOpener.hpp>
#include <swift2d/network/NetworkObjectBase.hpp>
#include <swift2d/network/ReplicationManager.hpp>
#include <swift2d/network/HttpConnection.hpp>

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

  uint64_t get_own_id();

  void connect(std::string const& game_ID);
  void disconnect();
  void update();

  template<typename T> void register_type(RakNet::RakString const& type) {
    peer_.replica_->register_object(type, [](){ return new T(); });
  };

  void distribute_object(NetworkObjectBase* object);

  bool is_host() const;

  friend class Singleton<Network>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void enter_phase(Phase phase);
  void register_game();
  void unregister_game();

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
