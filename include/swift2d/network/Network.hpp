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

#include <swift2d/math.hpp>
#include <swift2d/network/ReplicationManager.hpp>
#include <swift2d/network/HttpConnection.hpp>

#include <raknet/RPC3.h>

namespace RakNet {
  class RakPeerInterface;
  class ConnectionGraph2;
  class FullyConnectedMesh2;
  class NatPunchthroughClient;
  class NetworkIDManager;
}

namespace swift {

class NetworkObjectBase;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Network : public Singleton<Network> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  enum Phase {
    GETTING_EXTERNAL_IP,
    GETTING_INTERNAL_IP,
    READY
  };

  void update();

  template<typename T> void register_type(RakNet::RakString const& type) {
    replica_->register_object(type, [](){ return new T(); });
  };

  void connect(std::string const& other);
  void natpunch(math::uint64 uuid);

  void distribute_object(NetworkObjectBase* object);

  template<typename Function>
  void distribute_function(std::string const& function_name, Function const& f) {
    rpc_->RegisterFunction(function_name.c_str(), f);
  }

  template<typename ...Args>
  void call_function(std::string const& function_name, RakNet::NetworkID const& id, Args&& ... a) {
    rpc_->CallCPP(function_name.c_str(), id, a...);
  }

  std::string const& get_internal_address() const;
  std::string const& get_external_address() const;
  math::uint64       get_network_id() const;

  bool is_in_same_network(std::string const& other) const;

  friend class Singleton<Network>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Network();
  ~Network();

  RakNet::RakPeerInterface*      peer_;
  RakNet::NatPunchthroughClient* npt_;
  RakNet::NetworkIDManager*      id_manager_;
  RakNet::RPC3*                  rpc_;
  ReplicationManager*            replica_;

  Phase                          phase_;

  std::string                    nat_server_address_;

  std::string                    internal_id_;
  std::string                    external_id_;
};

}

#endif  // SWIFT2D_NETWORK_HPP
