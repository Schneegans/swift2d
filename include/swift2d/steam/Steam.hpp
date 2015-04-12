////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_STEAM_HPP
#define SWIFT2D_STEAM_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/properties.hpp>
#include <unordered_map>

struct LobbyChatUpdate_t;
struct LobbyChatMsg_t;
struct LobbyEnter_t;
struct PersonaStateChange_t;

namespace swift {

template<typename T> class SteamCallback;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Steam : public Singleton<Steam> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  enum MessageType {
    MSG_CHAT          = 0,
    MSG_CHAT_UPDATE   = 1,
    MSG_LEAVE         = 2,
    MSG_JOIN          = 3,
    MSG_MESSAGE       = 4,
    MSG_ERROR         = 5
  };

  struct RoomData {
    std::string name;
    int         max_players;
    int         current_players;
  };

  Signal<std::unordered_map<math::uint64, RoomData>> on_updated_room_list;
  Signal<MessageType, math::uint64, std::string>     on_message;

  bool init();
  void update();

  void         update_room_list();
  math::uint64 get_room_id();

  void         create_room(std::string const& name);
  void         join_room(math::uint64 room_id);
  void         leave_room();

  void         set_room_data(std::string const& key, std::string const& value);
  std::string  get_room_data(std::string const& key);

  void         set_user_data(std::string const& key, std::string const& value);
  std::string  get_user_data(std::string const& key, math::uint64 user);

  math::uint64 get_room_owner_id();
  std::string  get_internal_ip(math::uint64 user);
  std::string  get_external_ip(math::uint64 user);
  math::uint64 get_network_id(math::uint64 user);

  void         send_chat_message(std::string const& message);

  math::uint64 get_user_id();
  std::string  get_user_name();
  std::string  get_user_name(math::uint64 steam_id);
  std::string  get_user_avatar(math::uint64 steam_id);

  void         save_file_to_cloud(std::string const& file_name,
                                  std::string const& file_data);

  std::string  load_file_from_cloud(std::string const& file_name);

  friend class Singleton<Steam>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Steam();
  ~Steam();

  void save_avatar(math::uint64 steam_id);

  math::uint64 current_room_;

  SteamCallback<LobbyChatUpdate_t>*    chat_update_;
  SteamCallback<LobbyChatMsg_t>*       chat_message_;
  SteamCallback<LobbyEnter_t>*         lobby_enter_;
  SteamCallback<PersonaStateChange_t>* persona_change_;

  std::unordered_map<math::uint64, std::string> avatar_cache_;
};

}

#endif  // SWIFT2D_STEAM_HPP
