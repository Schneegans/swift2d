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

#include <unordered_map>

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

  Signal<std::unordered_map<uint64_t, std::string>> on_updated_room_list;

  void update();

  void update_room_list();
  void create_room(std::string const& name);
  void join_room(uint64_t id);
  void leave_room();

  // template<typename T> void register_type(RakNet::RakString const& type) {
  //   peer_.replica_->register_object(type, [](){ return new T(); });
  // };

  // void distribute_object(NetworkObjectBase* object);

  // bool is_host() const;

  friend class Singleton<SteamNetwork>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  SteamNetwork();
  ~SteamNetwork() {}

  SteamPeer  peer_;
  uint64_t   host_guid_;
};

}

#endif  // SWIFT2D_STEAM_NETWORK_HPP

