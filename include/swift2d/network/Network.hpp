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
class SWIFT_DLL Network : public Singleton<Network> {

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

  void update();

  template<typename T> void register_type(RakNet::RakString const& type) {
    replica_->register_object(type, [](){ return new T(); });
  };

  void distribute_object(NetworkObjectBase* object);

  std::string const& get_internal_address() const {
    return internal_id_;
  }

  std::string const& get_external_address() const {
    return external_id_;
  }

  bool is_in_same_network(std::string const& other) const;
  bool is_host() const;
  void connect(math::uint64 guid);

  friend class UpnpOpener;
  friend class Singleton<Network>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void enter_phase(Phase phase);

  Network();
  ~Network();

  void request_join(math::uint64 guid);
  void start_join(math::uint64 guid);
  void join(math::uint64 guid, std::string const& nat_server);

  RakNet::RakPeerInterface*             peer_;
  RakNet::ConnectionGraph2*             graph_;
  RakNet::FullyConnectedMesh2*          mesh_;
  RakNet::NatPunchthroughClient*        npt_;
  RakNet::NetworkIDManager*             id_manager_;

  ReplicationManager*                   replica_;

  UpnpOpener      upnp_;

  Phase           phase_;
  std::string     game_ID_;

  math::uint64    host_guid_;
  std::string     nat_server_address_;

  std::string    internal_id_;
  std::string    external_id_;
};

}

#endif  // SWIFT2D_NETWORK_HPP
