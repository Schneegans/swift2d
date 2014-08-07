////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/network/SteamNetwork.hpp>

#include <swift2d/network/ReplicationManager.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/math/operators.hpp>
#include <swift2d/steam/SteamOnceCallback.hpp>
#include <swift2d/steam/SteamCallback.hpp>

#include <raknet/src/MessageIdentifiers.h>
#include <raknet/src/RakNetTypes.h>
#include <raknet/src/RakPeerInterface.h>
#include <raknet/src/BitStream.h>
#include <raknet/src/FullyConnectedMesh2.h>
#include <raknet/src/PacketLogger.h>

#include <steam/steam_api.h>

#include <sstream>


namespace swift {

////////////////////////////////////////////////////////////////////////////////

SteamNetwork::SteamNetwork()
  : current_room_(0) {

  Logger::LOG_MESSAGE << "I'm " << peer_.get_guid() << std::endl;

  chat_update_ = new SteamCallback<LobbyChatUpdate_t>([](LobbyChatUpdate_t* result) {

    std::string who   (SteamFriends()->GetFriendPersonaName(result->m_ulSteamIDUserChanged));
    int         what  (result->m_rgfChatMemberStateChange);

    if (what == k_EChatMemberStateChangeEntered) {
      Logger::LOG_MESSAGE << who << " entered the room" << std::endl;
    }

    if (what == k_EChatMemberStateChangeLeft) {
      Logger::LOG_MESSAGE << who << " left the room" << std::endl;
    }

    if (what == k_EChatMemberStateChangeDisconnected) {
      Logger::LOG_MESSAGE << who << " disconnected" << std::endl;
    }

    if (what == k_EChatMemberStateChangeKicked) {
      Logger::LOG_MESSAGE << who << " was kicked" << std::endl;
    }

    if (what == k_EChatMemberStateChangeBanned) {
      Logger::LOG_MESSAGE << who << " was banned" << std::endl;
    }

  });

  chat_message_ = new SteamCallback<LobbyChatMsg_t>([this](LobbyChatMsg_t* result) {

    CSteamID speaker;
    EChatEntryType entryType;
    char data[2048];
    int cubData=sizeof(data);
    int length = SteamMatchmaking()->GetLobbyChatEntry(
      result->m_ulSteamIDLobby,
      result->m_iChatID, &speaker, data, cubData, &entryType);

    if (entryType == k_EChatEntryTypeChatMsg) {
      auto message = std::string(data, length);

      SteamNetwork::on_chat_message.emit(
        SteamFriends()->GetFriendPersonaName(speaker), message
      );
    }
  });

  lobby_enter_ = new SteamCallback<LobbyEnter_t>([this](LobbyEnter_t* result) {
    Logger::LOG_WARNING << "joined " << result->m_ulSteamIDLobby << std::endl;
    current_room_ = result->m_ulSteamIDLobby;

    int user_count = SteamMatchmaking()->GetNumLobbyMembers(current_room_);
    for (int i(0); i<user_count; ++i) {
      auto user = SteamMatchmaking()->GetLobbyMemberByIndex(current_room_, i);
      Logger::LOG_WARNING << "room mate " << i << " " << SteamFriends()->GetFriendPersonaName(user) << std::endl;
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

SteamNetwork::~SteamNetwork() {
  delete chat_update_;
  delete chat_message_;
  delete lobby_enter_;
}

////////////////////////////////////////////////////////////////////////////////

// void SteamNetwork::disconnect() {
//   if (phase_ == HOSTING_INSTANCE) {
//     Logger::LOG_MESSAGE << "Unregistering game." << std::endl;
//     // unregister_game();
//   }
// }

////////////////////////////////////////////////////////////////////////////////

// void SteamNetwork::connect() {
//   update_room_list();

  // http_.on_response.connect([&](std::string const& response) {
  //   switch (phase_) {

  //     // -----------------------------------------------------------------------
  //     case SEARCHING_FOR_OTHER_INSTANCES: {
  //       Logger::LOG_MESSAGE << "Got response from master server." << std::endl;

  //       bool found_other_game(false);

  //       try {
  //         boost::property_tree::ptree tree;
  //         std::istringstream is(response);
  //         boost::property_tree::read_json(is, tree);

  //         if (tree.get_child("GET").size() > 0) {
  //           host_guid_        = tree.get_child("GET").front().second.get<uint64_t>("guid");
  //           found_other_game  = true;
  //         }

  //       } catch(std::runtime_error const& e) {
  //         Logger::LOG_WARNING << "Failed to parse response: " << e.what() << std::endl;
  //       }

  //       if (found_other_game) enter_phase(CONNECTING_TO_HOST);
  //       else                  enter_phase(STARTING_NEW_INSTANCE);

  //       } break;

  //     // -----------------------------------------------------------------------
  //     case STARTING_NEW_INSTANCE:
  //       Logger::LOG_MESSAGE << "Successfully registered new instance." << std::endl;
  //       enter_phase(HOSTING_INSTANCE);
  //       break;

  //     // -----------------------------------------------------------------------
  //     case HOSTING_INSTANCE:
  //       // ignore reply
  //       break;


  //     // -----------------------------------------------------------------------
  //     default:
  //       Logger::LOG_WARNING << "Got unexpected HTTP response." << std::endl;
  //       break;
  //   }
  // });

  // enter_phase(CONNECTING_TO_SERVER);
// }

////////////////////////////////////////////////////////////////////////////////

void SteamNetwork::update() {

  SteamAPI_RunCallbacks();

  // auto register_new_peer = [&](RakNet::RakNetGUID guid){
  //   RakNet::Connection_RM3 *connection = peer_.replica_->AllocConnection(peer_.peer_->GetSystemAddressFromGuid(guid), guid);
  //   if (peer_.replica_->PushConnection(connection) == false) {
  //     peer_.replica_->DeallocConnection(connection);
  //   }
  // };

  // http_.update();

  // if (phase_ == HOSTING_INSTANCE && update_timer_.get_elapsed() > 20.0) {
  //   register_game();
  //   update_timer_.reset();
  // }

  for (RakNet::Packet* packet=peer_.peer_->Receive(); packet; peer_.peer_->DeallocatePacket(packet), packet=peer_.peer_->Receive()) {
    switch (packet->data[0]) {

      // ##################### BASIC PACKETS ###################################
      // -----------------------------------------------------------------------

      // case ID_CONNECTION_REQUEST_ACCEPTED:
      //   if (phase_ == CONNECTING_TO_SERVER) {

      //     enter_phase(SEARCHING_FOR_OTHER_INSTANCES);


      //   } else if (phase_ == CONNECTING_TO_HOST) {
      //     Logger::LOG_MESSAGE << "Connected to host " << packet->guid.ToString() << ". Sending join request." << std::endl;
      //     // peer_.request_join(packet->guid.g);

      //   } else if (phase_ == HOSTING_INSTANCE) {
      //     Logger::LOG_MESSAGE << "Connected to client " << packet->guid.ToString() << "." << std::endl;
      //   }
      //   break;

      // -----------------------------------------------------------------------
      // case ID_DISCONNECTION_NOTIFICATION:
      //   Logger::LOG_MESSAGE << packet->guid.ToString() << " disconnected." << std::endl;
      //   break;

      // ################## FULLY CONNECTED MESH ###############################
      // -----------------------------------------------------------------------
      // case ID_FCM2_NEW_HOST: {
      //   RakNet::BitStream bs(packet->data, packet->length, false);
      //   bs.IgnoreBytes(1);
      //   RakNet::RakNetGUID old_host;
      //   bs.Read(old_host);

      //   if (packet->guid.g == peer_.get_guid()) {
      //     if (phase_ != HOSTING_INSTANCE) {
      //       enter_phase(HOSTING_INSTANCE);
      //     }
      //   } else {
      //     Logger::LOG_MESSAGE << packet->guid.ToString() << " is host now." << std::endl;


      //     if (old_host != RakNet::UNASSIGNED_RAKNET_GUID) {
      //       Logger::LOG_MESSAGE << "Old host was " << old_host.ToString() << std::endl;
      //     } else {
      //       Logger::LOG_MESSAGE << "There was no host before." << std::endl;
      //     }
      //   }

      //   if (old_host == RakNet::UNASSIGNED_RAKNET_GUID) {
      //     DataStructures::List<RakNet::RakNetGUID> peers;
      //     peer_.mesh_->GetParticipantList(peers);
      //     for (unsigned int i=0; i < peers.Size(); i++) {
      //       register_new_peer(peers[i]);
      //     }
      //   }

      //   } break;

      // ##################### OTHER PACKETS ###################################
      // -----------------------------------------------------------------------
      default:
        Logger::LOG_TRACE << "Got " << RakNet::PacketLogger::BaseIDTOString(packet->data[0])
                          << " from " << packet->guid.ToString() << std::endl;
        break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void SteamNetwork::update_room_list() {
  SteamOnceCallback<LobbyMatchList_t>::set(
    SteamMatchmaking()->RequestLobbyList(),
    [](LobbyMatchList_t *result, bool f) {

    std::unordered_map<uint64_t, std::string> rooms;
    for (int i(0); i<result->m_nLobbiesMatching; ++i) {
      CSteamID id = SteamMatchmaking()->GetLobbyByIndex(i);
      rooms[id.ConvertToUint64()] = std::string(SteamMatchmaking()->GetLobbyData(id, "name"));
    }
    SteamNetwork::instance()->on_updated_room_list.emit(rooms);
  });
}

////////////////////////////////////////////////////////////////////////////////

void SteamNetwork::create_room(std::string const& name) {
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

void SteamNetwork::join_room(uint64_t id) {
  if (current_room_ == 0) {
    SteamMatchmaking()->JoinLobby(id);
  } else {
    Logger::LOG_WARNING << "Already in a room" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void SteamNetwork::leave_room() {
  if (current_room_ != 0) {

    Logger::LOG_WARNING << "left " << current_room_ << std::endl;
    SteamMatchmaking()->LeaveLobby(current_room_);
    current_room_ = 0;

  } else {
    Logger::LOG_WARNING << "Not in a room" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void SteamNetwork::send_chat_message(std::string const& message) {
  if (current_room_ != 0) {

    if (!SteamMatchmaking()->SendLobbyChatMsg(current_room_, message.c_str(), message.length()+1)) {
      Logger::LOG_WARNING << "failed to send message" << std::endl;
    }

  } else {
    Logger::LOG_WARNING << "Not in a room" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

// void SteamNetwork::distribute_object(NetworkObjectBase* object) {
//   peer_.replica_->Reference(object);
// }

////////////////////////////////////////////////////////////////////////////////

// bool SteamNetwork::is_host() const {
//   return peer_.mesh_->IsConnectedHost();
// }

////////////////////////////////////////////////////////////////////////////////

// void SteamNetwork::enter_phase(Phase phase) {
//   phase_ = phase;

//   switch (phase) {

//     // -------------------------------------------------------------------------
//     case CONNECTING_TO_SERVER:
//       Logger::LOG_MESSAGE << "Connecting to NAT server..." << std::endl;
//       // peer_.connect("natpunch.jenkinssoftware.com", 61111);
//       break;

//     // -------------------------------------------------------------------------
//     case SEARCHING_FOR_OTHER_INSTANCES:
//       Logger::LOG_MESSAGE << "Downloading running instances from "
//                           << "master server..." << std::endl;
//       // http_.get("masterserver2.raknet.com/testServer?__gameId=" + game_ID_,
//       //           "masterserver2.raknet.com", 80);
//       break;

//     // -------------------------------------------------------------------------
//     case CONNECTING_TO_HOST:
//       Logger::LOG_MESSAGE << "Found running instance. Searching route to host "
//                           << host_guid_ << "..." << std::endl;
//       // peer_.open_nat(host_guid_, nat_server_address_);
//       break;

//     // -------------------------------------------------------------------------
//     case STARTING_NEW_INSTANCE:
//       Logger::LOG_MESSAGE << "No running instances found. "
//                           << "Starting new instance..." << std::endl;
//       // peer_.mesh_->ResetHostCalculation();
//       // register_game();
//       break;

//     // -------------------------------------------------------------------------
//     case HOSTING_INSTANCE:
//       Logger::LOG_MESSAGE << "I'm host now." << std::endl;
//       // register_game();
//       break;

//     // -------------------------------------------------------------------------
//     case PARTICIPATING:
//       Logger::LOG_MESSAGE << "Participating game." << std::endl;
//       break;
//   }
// }

////////////////////////////////////////////////////////////////////////////////

}

