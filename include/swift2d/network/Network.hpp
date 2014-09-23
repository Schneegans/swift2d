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

#include <swift2d/network/UpnpOpener.hpp>
#include <swift2d/network/NetworkObjectBase.hpp>
#include <swift2d/network/ReplicationManager.hpp>
#include <swift2d/network/HttpConnection.hpp>

namespace RakNet {
  class RakPeerInterface;
  class ConnectionGraph2;
  class FullyConnectedMesh2;
  class NatPunchthroughClient;
  class NetworkIDManager;
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
    STARTED,
    CONNECTING_TO_NAT_SERVER,
    NAT_PUNCH_TO_HOST,
    CONNECTING_TO_HOST
  };

  enum PacketID {
    REQUEST_JOIN
  };

  uint64_t get_own_id();

  void update();

  template<typename T> void register_type(RakNet::RakString const& type) {
    replica_->register_object(type, [](){ return new T(); });
  };

  void distribute_object(NetworkObjectBase* object);

  bool is_host() const;
  void connect(std::string const& ip, unsigned short port);
  void open_nat(uint64_t guid, std::string const& nat_server);

  friend class UpnpOpener;
  friend class Singleton<Network>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void enter_phase(Phase phase);

  Network();
  ~Network();

  void request_join(uint64_t guid);
  void start_join(uint64_t guid);
  void join(uint64_t guid, std::string const& nat_server);

  RakNet::RakPeerInterface*             peer_;
  RakNet::ConnectionGraph2*             graph_;
  RakNet::FullyConnectedMesh2*          mesh_;
  RakNet::NatPunchthroughClient*        npt_;
  RakNet::NetworkIDManager*             id_manager_;

  ReplicationManager*                   replica_;

  UpnpOpener      upnp_;

  Phase           phase_;
  std::string     game_ID_;

  uint64_t        host_guid_;
  std::string     nat_server_address_;
};

}

#endif  // SWIFT2D_NETWORK_HPP
