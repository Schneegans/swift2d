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


struct LobbyChatUpdate_t;
struct LobbyChatMsg_t;
struct LobbyEnter_t;


namespace swift {

template<typename T> class SteamCallback;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SteamNetwork : public Singleton<SteamNetwork> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Signal<std::unordered_map<uint64_t, std::string>> on_updated_room_list;
  Signal<std::string, std::string>                  on_chat_message;

  void update();

  void update_room_list();
  void create_room(std::string const& name);
  void join_room(uint64_t id);
  void leave_room();

  void send_chat_message(std::string const& message);

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
  ~SteamNetwork();

  SteamPeer  peer_;
  uint64_t   current_room_;

  SteamCallback<LobbyChatUpdate_t>* chat_update_;
  SteamCallback<LobbyChatMsg_t>*    chat_message_;
  SteamCallback<LobbyEnter_t>*      lobby_enter_;
};

}

#endif  // SWIFT2D_STEAM_NETWORK_HPP

