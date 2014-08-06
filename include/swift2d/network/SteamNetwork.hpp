////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_STEAM_NETWORK_HPP
#define SWIFT2D_STEAM_NETWORK_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>

#include <swift2d/network/SteamPeer.hpp>
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
class SteamNetwork : public Singleton<SteamNetwork> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  enum Phase {
    CONNECTING_TO_SERVER,
    DETECT_NAT_TYPE,
    OPENING_UPNP,
    SEARCHING_FOR_OTHER_INSTANCES,
    CONNECTING_TO_HOST,
    STARTING_NEW_INSTANCE,
    HOSTING_INSTANCE,
    CONNECTING_TO_PEERS,
    PARTICIPATING
  };

  void connect(std::string const& game_ID);
  void disconnect();
  void update();

  template<typename T> void register_type(RakNet::RakString const& type) {
    peer_.replica_->register_object(type, [](){ return new T(); });
  };

  void distribute_object(NetworkObjectBase* object);

  bool is_host() const;

  friend class Singleton<SteamNetwork>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void enter_phase(Phase phase);

  SteamNetwork();
  ~SteamNetwork() {}

  SteamPeer       peer_;
  HttpConnection  http_;

  Phase           phase_;
  std::string     game_ID_;

  Timer           update_timer_;
  uint64_t        host_guid_;
};

}

#endif  // SWIFT2D_STEAM_NETWORK_HPP

