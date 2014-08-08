////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/steam/Steam.hpp>

#include <swift2d/Swift2D.hpp>
#include <swift2d/steam/SteamOnceCallback.hpp>
#include <swift2d/steam/SteamCallback.hpp>
#include <swift2d/utils/Logger.hpp>

#include <stb_image/stb_image_write.h>

#include <iostream>
#include <steam/steam_api.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Steam::Steam()
  : current_room_(0) {

  // on chat update ------------------------------------------------------------
  chat_update_ = new SteamCallback<LobbyChatUpdate_t>([this](LobbyChatUpdate_t* result) {

    std::string what;

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeEntered) {
      what = "entered the room";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeLeft) {
      what = "left the room";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeDisconnected) {
      what = "disconnected";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeKicked) {
      what = "was kicked";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeBanned) {
      what = "was banned";
    }

    on_message.emit(MessageType::CHAT_UPDATE, result->m_ulSteamIDUserChanged, what);

  });

  // on chat message -----------------------------------------------------------
  chat_message_ = new SteamCallback<LobbyChatMsg_t>([this](LobbyChatMsg_t* result) {
    CSteamID        speaker;
    EChatEntryType  type;
    char            data[2048];
    int             length = SteamMatchmaking()->GetLobbyChatEntry(
                                result->m_ulSteamIDLobby, result->m_iChatID,
                                &speaker, data, sizeof(data), &type);

    if (type == k_EChatEntryTypeChatMsg) {
      auto message = std::string(data, length);
      on_message.emit(MessageType::CHAT, speaker.ConvertToUint64(), message);
    }
  });

  // on lobby enter ------------------------------------------------------------
  lobby_enter_ = new SteamCallback<LobbyEnter_t>([this](LobbyEnter_t* result) {
    current_room_ = result->m_ulSteamIDLobby;
    std::string name(SteamMatchmaking()->GetLobbyData(current_room_, "name"));

    on_message.emit(MessageType::JOIN, get_id(), "joined " + name);

    int user_count = SteamMatchmaking()->GetNumLobbyMembers(current_room_);
    for (int i(0); i<user_count; ++i) {
      auto user = SteamMatchmaking()->GetLobbyMemberByIndex(current_room_, i);
      on_message.emit(MessageType::CHAT_UPDATE, user.ConvertToUint64(), "is in this room");
    }
  });

  // persona state change ------------------------------------------------------
  persona_change_ = new SteamCallback<PersonaStateChange_t>([this](PersonaStateChange_t* result) {

    // save avatar
    if (result->m_nChangeFlags == k_EPersonaChangeAvatar) {

      auto avatar(avatar_cache_.find(result->m_ulSteamID));
      std::string file;

      if (avatar == avatar_cache_.end()) {
        file = Swift2D::instance()->get_tmp_file("png");
        avatar_cache_[result->m_ulSteamID] = file;
      } else {
        file = avatar->second;
      }

      save_avatar(result->m_ulSteamID);
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

Steam::~Steam() {
  Logger::LOG_MESSAGE << "Shutting down steam connection." << std::endl;
  SteamAPI_Shutdown();

  delete chat_update_;
  delete chat_message_;
  delete lobby_enter_;
}

////////////////////////////////////////////////////////////////////////////////

void Steam::init() {
  if (SteamAPI_Init()) {
    Logger::LOG_MESSAGE << "Initialized Steam." << std::endl;
  } else {
    Logger::LOG_WARNING << "Failed to initialized Steam!" << std::endl;
  }

  if (SteamAPI_IsSteamRunning()) {
    Logger::LOG_MESSAGE << "Steam is running. Logged in as " << SteamFriends()->GetPersonaName() << "." << std::endl;
  } else {
    Logger::LOG_WARNING << "Steam is not running." << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Steam::update() {
  SteamAPI_RunCallbacks();
}

////////////////////////////////////////////////////////////////////////////////

void Steam::update_room_list() {
  SteamOnceCallback<LobbyMatchList_t>::set(
    SteamMatchmaking()->RequestLobbyList(),
    [](LobbyMatchList_t *result, bool f) {

    std::unordered_map<uint64_t, std::string> rooms;
    for (int i(0); i<result->m_nLobbiesMatching; ++i) {
      CSteamID id = SteamMatchmaking()->GetLobbyByIndex(i);
      rooms[id.ConvertToUint64()] = std::string(SteamMatchmaking()->GetLobbyData(id, "name"));
    }
    Steam::instance()->on_updated_room_list.emit(rooms);
  });
}

////////////////////////////////////////////////////////////////////////////////

void Steam::create_room(std::string const& name) {
  if (current_room_ == 0) {

    SteamOnceCallback<LobbyCreated_t>::set(
    SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 8),
    [this, name](LobbyCreated_t *result, bool f) {

      if (result->m_eResult == k_EResultOK) {
        Logger::LOG_WARNING << "created " << result->m_ulSteamIDLobby << std::endl;
        current_room_ = result->m_ulSteamIDLobby;

        SteamMatchmaking()->SetLobbyData(current_room_, "name", name.c_str());
      } else {
        Logger::LOG_WARNING << "failed to create lobby" << std::endl;
      }
    });

  } else {
    Logger::LOG_WARNING << "Already in a room" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Steam::join_room(uint64_t id) {
  if (current_room_ != 0) {
    leave_room();
  }
  SteamMatchmaking()->JoinLobby(id);
}

////////////////////////////////////////////////////////////////////////////////

void Steam::leave_room() {
  if (current_room_ != 0) {

    std::string name(SteamMatchmaking()->GetLobbyData(current_room_, "name"));
    on_message.emit(MessageType::LEAVE, get_id(), "left " + name);

    SteamMatchmaking()->LeaveLobby(current_room_);
    current_room_ = 0;

  } else {
    Logger::LOG_WARNING << "Not in a room" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Steam::send_chat_message(std::string const& message) {
  if (current_room_ != 0) {

    if (!SteamMatchmaking()->SendLobbyChatMsg(current_room_, message.c_str(), message.length()+1)) {
      Logger::LOG_WARNING << "failed to send message" << std::endl;
    }

  } else {
    Logger::LOG_WARNING << "Not in a room" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

uint64_t Steam::get_id() {
  return SteamUser()->GetSteamID().ConvertToUint64();
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::get_name() {
  return SteamFriends()->GetPersonaName();
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::get_name(uint64_t steam_id) {
  return SteamFriends()->GetFriendPersonaName(steam_id);
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::get_avatar(uint64_t steam_id) {
  auto avatar(avatar_cache_.find(steam_id));
  if (avatar != avatar_cache_.end()) {
    return avatar->second;
  }

  SteamFriends()->RequestUserInformation(steam_id, false);

  std::string file(Swift2D::instance()->get_tmp_file("png"));
  avatar_cache_[steam_id] = file;

  save_avatar(steam_id);

  return file;
}

////////////////////////////////////////////////////////////////////////////////

void Steam::save_avatar(uint64_t steam_id) {
  int avatar_id = SteamFriends()->GetSmallFriendAvatar(steam_id);

  uint32 width;
  uint32 height;

  SteamUtils()->GetImageSize(avatar_id, &width, &height);

  int data_length = width*height*4*sizeof(uint8);
  uint8 data[data_length];

  if (!SteamUtils()->GetImageRGBA(avatar_id, data, data_length)) {
    Logger::LOG_WARNING << "Failed to get avatar image!" << std::endl;
  } else {
    stbi_write_png(avatar_cache_[steam_id].c_str(), width, height, 4, data, width*4*sizeof(uint8));
  }
}

////////////////////////////////////////////////////////////////////////////////

}
