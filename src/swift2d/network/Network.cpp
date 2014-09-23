////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/network/Network.hpp>

#include <swift2d/network/ReplicationManager.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/math/operators.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <raknet/src/MessageIdentifiers.h>
#include <raknet/src/RakNetTypes.h>
#include <raknet/src/RakPeerInterface.h>
#include <raknet/src/BitStream.h>
#include <raknet/src/FullyConnectedMesh2.h>
#include <raknet/src/PacketLogger.h>
#include <raknet/src/NatTypeDetectionClient.h>
#include <raknet/src/ConnectionGraph2.h>
#include <raknet/src/NatPunchthroughClient.h>
#include <raknet/src/NetworkIDManager.h>

#include <sstream>

#define USE_UPNP false

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Network::Network()
  : phase_(STARTED)
  , peer_             (RakNet::RakPeerInterface::GetInstance())
  , graph_            (RakNet::ConnectionGraph2::GetInstance())
  , mesh_             (RakNet::FullyConnectedMesh2::GetInstance())
  , npt_              (RakNet::NatPunchthroughClient::GetInstance())
  , id_manager_       (new RakNet::NetworkIDManager())
  , replica_          (new ReplicationManager()) {

  peer_->AttachPlugin(mesh_);
  peer_->AttachPlugin(graph_);
  peer_->AttachPlugin(npt_);
  peer_->AttachPlugin(replica_);

  replica_->SetNetworkIDManager(id_manager_);
  replica_->SetAutoManageConnections(false,true);

  mesh_->SetAutoparticipateConnections(false);
  mesh_->SetConnectOnNewRemoteConnection(false, "");

  RakNet::SocketDescriptor sd;
  sd.socketFamily = AF_INET;
  sd.port = 0;

  RakNet::StartupResult sr = peer_->Startup(8, &sd, 1);

  if (sr != RakNet::RAKNET_STARTED) {
    Logger::LOG_ERROR << "Failed to start peer!" << std::endl;
  }

  peer_->SetMaximumIncomingConnections(8);
  peer_->SetTimeoutTime(1000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

  // connect("natpunch.jenkinssoftware.com", 61111);

  Logger::LOG_MESSAGE << "I'm " << get_own_id() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

Network::~Network() {
  peer_->Shutdown(100);
  RakNet::RakPeerInterface::DestroyInstance(peer_);
  RakNet::ConnectionGraph2::DestroyInstance(graph_);
  RakNet::FullyConnectedMesh2::DestroyInstance(mesh_);

  delete replica_;
  delete id_manager_;
}

////////////////////////////////////////////////////////////////////////////////

uint64_t Network::get_own_id() {
  return peer_->GetMyGUID().g;
}

////////////////////////////////////////////////////////////////////////////////

void Network::connect(uint64_t guid) {
  if (phase_ == STARTED) {
    host_guid_ = guid;
    phase_ = CONNECTING_TO_NAT_SERVER;

    RakNet::ConnectionAttemptResult car = peer_->Connect("natpunch.jenkinssoftware.com", 61111, 0, 0);
    if (car!=RakNet::CONNECTION_ATTEMPT_STARTED) {
      Logger::LOG_WARNING << "Failed to connect! Code=" << car << std::endl;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

  // upnp_.on_success.connect([&](){
  //   Logger::LOG_MESSAGE << "Successfully opened UPNP." << std::endl;
  //   enter_phase(SEARCHING_FOR_OTHER_INSTANCES);
  // });

  // upnp_.on_fail.connect([&](){
  //   Logger::LOG_MESSAGE << "Failed to open UPNP. Using NAT punch through." << std::endl;
  //   enter_phase(OPENING_UPNP);
  // });

  // enter_phase(HOSTING_INSTANCE);


////////////////////////////////////////////////////////////////////////////////

void Network::update() {

  // auto register_new_peer = [&](RakNet::RakNetGUID guid){
  //   RakNet::Connection_RM3 *connection = replica_->AllocConnection(peer_->GetSystemAddressFromGuid(guid), guid);
  //   if (replica_->PushConnection(connection) == false) {
  //     replica_->DeallocConnection(connection);
  //   }
  // };

  for (RakNet::Packet* packet=peer_->Receive(); packet; peer_->DeallocatePacket(packet), packet=peer_->Receive()) {
    switch (packet->data[0]) {

      // ##################### BASIC PACKETS ###################################
      // -----------------------------------------------------------------------
      case ID_CONNECTION_REQUEST_ACCEPTED:

        if (phase_ == CONNECTING_TO_NAT_SERVER) {

          // the nat server accepted our connection
          Logger::LOG_MESSAGE << "Connected to NAT server." << std::endl;
          nat_server_address_ = packet->systemAddress.ToString();
          phase_ = NAT_PUNCH_TO_HOST;

          npt_->OpenNAT(RakNet::RakNetGUID(host_guid_), RakNet::SystemAddress(nat_server_address_.c_str()));

        } else if (phase_ == NAT_PUNCH_TO_HOST) {

          // nar punch was successfull, we got a connection!
          phase_ = CONNECTING_TO_HOST;
          // request_join(packet->guid.g);
        }

        break;

      // // -----------------------------------------------------------------------
      // case ID_DISCONNECTION_NOTIFICATION:
      //   Logger::LOG_MESSAGE << packet->guid.ToString() << " disconnected." << std::endl;
      //   break;

      // // ################ NAT PUNCH THROUGH PACKETS ############################
      // // -----------------------------------------------------------------------
      // case ID_NAT_PUNCHTHROUGH_SUCCEEDED:
      //   if (phase_ == CONNECTING_TO_HOST || phase_ == CONNECTING_TO_PEERS) {
      //     Logger::LOG_MESSAGE << "NAT punch succeeded. Connecting..." << std::endl;
      //     connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort());
      //   }
      //   break;

      // // -----------------------------------------------------------------------
      // case ID_NAT_PUNCHTHROUGH_FAILED:
      //   Logger::LOG_MESSAGE << "NAT punch through failed." << std::endl;
      //   break;

      // // -----------------------------------------------------------------------
      // case ID_NAT_TYPE_DETECTION_RESULT: {
      //   auto type = (RakNet::NATTypeDetectionResult) packet->data[1];
      //   Logger::LOG_MESSAGE << "NAT Type is " << RakNet::NATTypeDetectionResultToString(type) << " (" << RakNet::NATTypeDetectionResultToStringFriendly(type) << ")" << std::endl;

      //   if (USE_UPNP) enter_phase(OPENING_UPNP);
      //   else          enter_phase(SEARCHING_FOR_OTHER_INSTANCES);
      //   } break;

      // // ################## FULLY CONNECTED MESH ###############################
      // // -----------------------------------------------------------------------
      // case ID_FCM2_NEW_HOST: {
      //   RakNet::BitStream bs(packet->data, packet->length, false);
      //   bs.IgnoreBytes(1);
      //   RakNet::RakNetGUID old_host;
      //   bs.Read(old_host);

      //   if (packet->guid.g == get_own_id()) {
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
      //     mesh_->GetParticipantList(peers);
      //     for (unsigned int i=0; i < peers.Size(); i++) {
      //       register_new_peer(peers[i]);
      //     }
      //   }

      //   } break;

      // // -----------------------------------------------------------------------
      // case (ID_USER_PACKET_ENUM + REQUEST_JOIN):
      //   Logger::LOG_MESSAGE << "Got join request from " << packet->guid.ToString() << "." << std::endl;
      //   start_join(packet->guid.g);
      //   break;

      // // -----------------------------------------------------------------------
      // case ID_FCM2_VERIFIED_JOIN_CAPABLE:
      //   mesh_->RespondOnVerifiedJoinCapable(packet, true, 0);
      //   break;

      // // -----------------------------------------------------------------------
      // case ID_FCM2_VERIFIED_JOIN_ACCEPTED: {
      //   DataStructures::List<RakNet::RakNetGUID> peers;
      //   bool this_was_accepted;
      //   mesh_->GetVerifiedJoinAcceptedAdditionalData(packet, &this_was_accepted, peers, 0);
      //   if (this_was_accepted) {
      //     Logger::LOG_MESSAGE << "Join accepted." << std::endl;
      //   } else {
      //     Logger::LOG_MESSAGE << "Peer " << peers[0].ToString() << " joined the game." << std::endl;
      //   }

      //   if (mesh_->GetConnectedHost() != RakNet::UNASSIGNED_RAKNET_GUID) {
      //     for (unsigned int i=0; i < peers.Size(); i++) {
      //       register_new_peer(peers[i]);
      //     }
      //   }

      //   if (this_was_accepted) {
      //     enter_phase(PARTICIPATING);
      //   }

      // } break;

      // // -----------------------------------------------------------------------
      // case ID_FCM2_VERIFIED_JOIN_REJECTED:
      //   Logger::LOG_MESSAGE << "Join rejected." << std::endl;
      //   peer_->CloseConnection(packet->guid, true);
      //   break;

      // // -----------------------------------------------------------------------
      // case ID_FCM2_VERIFIED_JOIN_FAILED:
      //   Logger::LOG_MESSAGE << "Join failed." << std::endl;
      //   break;

      // // -----------------------------------------------------------------------
      // case ID_FCM2_VERIFIED_JOIN_START:
      //   Logger::LOG_MESSAGE << "Connecting to other peers..." << std::endl;
      //   enter_phase(CONNECTING_TO_PEERS);
      //   join(packet->guid.g, nat_server_address_);
      //   break;

      // ##################### OTHER PACKETS ###################################
      // -----------------------------------------------------------------------
      default:
        Logger::LOG_DEBUG << "Got " << RakNet::PacketLogger::BaseIDTOString(packet->data[0])
                          << " from " << packet->guid.ToString() << std::endl;
        break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Network::distribute_object(NetworkObjectBase* object) {
  replica_->Reference(object);
}

// ////////////////////////////////////////////////////////////////////////////////

bool Network::is_host() const {
  return mesh_->IsConnectedHost();
}

////////////////////////////////////////////////////////////////////////////////

void Network::enter_phase(Phase phase) {
  // phase_ = phase;

  // switch (phase) {

  //   // -------------------------------------------------------------------------
  //   case CONNECTING_TO_SERVER:
  //     Logger::LOG_MESSAGE << "Connecting to NAT server..." << std::endl;
  //     connect("natpunch.jenkinssoftware.com", 61111);
  //     break;

  //   // -------------------------------------------------------------------------
  //   case OPENING_UPNP:
  //     Logger::LOG_MESSAGE << "Opening UPNP..." << std::endl;
  //     // upnp_.open(peer_);
  //     break;

  //   // -------------------------------------------------------------------------
  //   case SEARCHING_FOR_OTHER_INSTANCES:
  //     Logger::LOG_MESSAGE << "Downloading running instances from "
  //                         << "master server..." << std::endl;
  //     http_.get("masterserver2.raknet.com/testServer?__gameId=" + game_ID_,
  //               "masterserver2.raknet.com", 80);
  //     break;

  //   // -------------------------------------------------------------------------
  //   case CONNECTING_TO_HOST:
  //     Logger::LOG_MESSAGE << "Found running instance. Searching route to host "
  //                         << host_guid_ << "..." << std::endl;
  //     open_nat(host_guid_, nat_server_address_);
  //     break;

  //   // -------------------------------------------------------------------------
  //   case STARTING_NEW_INSTANCE:
  //     Logger::LOG_MESSAGE << "No running instances found. "
  //                         << "Starting new instance..." << std::endl;
  //     mesh_->ResetHostCalculation();
  //     // register_game();
  //     // update_timer_.reset();
  //     break;

  //   // -------------------------------------------------------------------------
  //   case HOSTING_INSTANCE:
  //     Logger::LOG_MESSAGE << "I'm host now." << std::endl;
  //     // register_game();
  //     // update_timer_.reset();
  //     break;

  //   // -------------------------------------------------------------------------
  //   case PARTICIPATING:
  //     Logger::LOG_MESSAGE << "Participating game." << std::endl;
  //     break;
  // }
}

////////////////////////////////////////////////////////////////////////////////

// void Network::request_join(uint64_t guid) {
//   mesh_->ResetHostCalculation();

//   RakNet::BitStream message;
//   message.Write((RakNet::MessageID)(ID_USER_PACKET_ENUM + Network::REQUEST_JOIN));
//   peer_->Send(&message, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::RakNetGUID(guid), false);
// }

// ////////////////////////////////////////////////////////////////////////////////

// void Network::start_join(uint64_t guid) {
//   mesh_->StartVerifiedJoin(RakNet::RakNetGUID(guid));
// }

// ////////////////////////////////////////////////////////////////////////////////

// void Network::join(uint64_t guid, std::string const& nat_server) {
//   DataStructures::List<RakNet::SystemAddress> addresses;
//   DataStructures::List<RakNet::RakNetGUID> guids;
//   DataStructures::List<RakNet::BitStream*> userData;
//   mesh_->GetVerifiedJoinRequiredProcessingList(RakNet::RakNetGUID(guid), addresses, guids, userData);
//   for (unsigned int i=0; i < guids.Size(); i++) {
//     // if (guids[i].g != get_guid()) {
//       npt_->OpenNAT(guids[i], RakNet::SystemAddress(nat_server.c_str()));
//     // }
//   }
// }

////////////////////////////////////////////////////////////////////////////////

}
