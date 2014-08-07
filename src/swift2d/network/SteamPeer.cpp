////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/network/SteamPeer.hpp>

#include <swift2d/network/SteamNetwork.hpp>
#include <swift2d/network/ReplicationManager.hpp>
#include <swift2d/utils/Logger.hpp>

#include <raknet/src/ConnectionGraph2.h>
#include <raknet/src/FullyConnectedMesh2.h>
#include <raknet/src/RakPeerInterface.h>
#include <raknet/src/BitStream.h>
#include <raknet/src/MessageIdentifiers.h>
#include <raknet/src/NetworkIDManager.h>

#include <steam/steam_api.h>

#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SteamPeer::SteamPeer()
  : peer_             (RakNet::RakPeerInterface::GetInstance())
  , graph_            (RakNet::ConnectionGraph2::GetInstance())
  , mesh_             (RakNet::FullyConnectedMesh2::GetInstance())
  , id_manager_       (new RakNet::NetworkIDManager())
  , steam_            (RakNet::Lobby2Client_Steam::GetInstance())
  , steam_messages_   (new RakNet::Lobby2MessageFactory_Steam())
  , replica_          (new ReplicationManager()) {

  peer_->AttachPlugin(mesh_);
  peer_->AttachPlugin(graph_);
  peer_->AttachPlugin(replica_);
  peer_->AttachPlugin(steam_);

  steam_->AddCallbackInterface(this);
  steam_->SetMessageFactory(steam_messages_);

  replica_->SetNetworkIDManager(id_manager_);
  replica_->SetAutoManageConnections(false,true);

  // mesh_->SetAutoparticipateConnections(false);
  mesh_->SetConnectOnNewRemoteConnection(false, "");

  RakNet::SocketDescriptor sd;
  sd.socketFamily=AF_INET;
  sd.port=0;

  RakNet::StartupResult sr = peer_->Startup(32,&sd,1);

  if (sr != RakNet::RAKNET_STARTED) {
    Logger::LOG_ERROR << "Failed to start peer!" << std::endl;
  }

  peer_->SetMaximumIncomingConnections(32);
  peer_->SetTimeoutTime(1000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

  RakNet::Lobby2Message* msg = steam_messages_->Alloc(RakNet::L2MID_Client_Login);
  steam_->SendMsg(msg);
  if (msg->resultCode != RakNet::L2RC_PROCESSING &&
      msg->resultCode != RakNet::L2RC_SUCCESS) {

    Logger::LOG_ERROR << "Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
    Logger::LOG_ERROR << "If this fails, steam_appid.txt was probably not in the working directory." << std::endl;
  }

  steam_messages_->Dealloc(msg);
}

////////////////////////////////////////////////////////////////////////////////

SteamPeer::~SteamPeer() {
  RakNet::Lobby2Message* log_off = steam_messages_->Alloc(RakNet::L2MID_Client_Logoff);
  steam_->SendMsg(log_off);
  steam_messages_->Dealloc(log_off);

  peer_->Shutdown(100);
  RakNet::RakPeerInterface::DestroyInstance(peer_);
  RakNet::ConnectionGraph2::DestroyInstance(graph_);
  RakNet::FullyConnectedMesh2::DestroyInstance(mesh_);
  RakNet::Lobby2Client_Steam::DestroyInstance(steam_);

  delete replica_;
  delete id_manager_;
  delete steam_messages_;
}

////////////////////////////////////////////////////////////////////////////////

uint64_t SteamPeer::get_guid() const {
  return peer_->GetMyGUID().g;
}

////////////////////////////////////////////////////////////////////////////////

void SteamPeer::MessageResult(RakNet::Notification_Console_MemberJoinedRoom *message) {
  RakNet::Notification_Console_MemberJoinedRoom_Steam *msgSteam = (RakNet::Notification_Console_MemberJoinedRoom_Steam *) message;
  RakNet::RakString msg;
  msgSteam->DebugMsg(msg);
  Logger::LOG_MESSAGE << msg.C_String() << std::endl;
  // Guy with the lower ID connects to the guy with the higher ID
  uint64_t mySteamId=SteamUser()->GetSteamID().ConvertToUint64();
  if (mySteamId < msgSteam->srcMemberId) {
    // Steam's NAT punch is implicit, so it takes a long time to connect. Give it extra time
    unsigned int sendConnectionAttemptCount=24;
    unsigned int timeBetweenSendConnectionAttemptsMS=500;
    RakNet::ConnectionAttemptResult car = peer_->Connect(msgSteam->remoteSystem.ToString(false), msgSteam->remoteSystem.GetPort(), 0, 0, 0, 0, sendConnectionAttemptCount, timeBetweenSendConnectionAttemptsMS);
    RakAssert(car==CONNECTION_ATTEMPT_STARTED);
  }
}

////////////////////////////////////////////////////////////////////////////////

void SteamPeer::MessageResult(RakNet::Console_SearchRooms *message) {
  RakNet::Console_SearchRooms_Steam *msg = (RakNet::Console_SearchRooms_Steam *) message;

  std::unordered_map<uint64_t, std::string> rooms;

  for (int i(0); i<msg->roomIds.GetSize(); ++i) {
    rooms[msg->roomIds[i]] = msg->roomNames[i];
  }

  SteamNetwork::instance()->on_updated_room_list.emit(rooms);
}

////////////////////////////////////////////////////////////////////////////////

void SteamPeer::ExecuteDefaultResult(RakNet::Lobby2Message *message) {
  RakNet::RakString out;
  message->DebugMsg(out);
  Logger::LOG_MESSAGE << "ExecuteDefaultResult: " << out.C_String() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

}

