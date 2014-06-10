////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "Network.hpp"

#include <swift2d/utils/Logger.hpp>

#include <../../third_party/raknet/src/MessageIdentifiers.h>
#include <../../third_party/raknet/src/RakNetTypes.h>
#include <../../third_party/raknet/src/RakPeerInterface.h>
#include <../../third_party/raknet/src/BitStream.h>
#include <../../third_party/raknet/src/FullyConnectedMesh2.h>
#include <../../third_party/raknet/src/UDPProxyClient.h>
#include <../../third_party/raknet/src/PacketLogger.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Network::Network()
  : phase_(SEARCHING_FOR_OTHER_INSTANCES) {

  update_timer_.start();

  Logger::LOG_MESSAGE << "We are " << peer_.get_guid() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void Network::connect(std::string const& game_ID) {
  game_ID_ = game_ID;

  http_.on_response.connect([&](std::string const& response) {
    switch (phase_) {

      // -----------------------------------------------------------------------
      case SEARCHING_FOR_OTHER_INSTANCES: {
        Logger::LOG_MESSAGE << "Got response from master server." << std::endl;

        bool found_other_game(false);

        try {
          boost::property_tree::ptree tree;
          std::istringstream is(response);
          boost::property_tree::read_json(is, tree);

          if (tree.get_child("GET").size() > 0) {
            host_guid_        = tree.get_child("GET").front().second.get<uint64_t>("guid");
            found_other_game  = true;
          }

        } catch(std::runtime_error const& e) {
          Logger::LOG_WARNING << "Failed to parse response: " << e.what() << std::endl;
        }

        if (found_other_game) enter_phase(CONNECTING_TO_HOST);
        else                  enter_phase(STARTING_NEW_INSTANCE);

        } break;

      // -----------------------------------------------------------------------
      case STARTING_NEW_INSTANCE:
        Logger::LOG_MESSAGE << "Successfully registered new instance." << std::endl;
        enter_phase(HOSTING_INSTANCE);
        break;

      // -----------------------------------------------------------------------
      case HOSTING_INSTANCE:
        // ignore reply
        break;


      // -----------------------------------------------------------------------
      default:
        Logger::LOG_WARNING << "Got unexpected HTTP response." << std::endl;
        break;
    }
  });

  upnp_.on_success.connect([&](){
    Logger::LOG_MESSAGE << "Successfully opened UPNP." << std::endl;
    enter_phase(SEARCHING_FOR_OTHER_INSTANCES);
  });

  upnp_.on_fail.connect([&](){
    Logger::LOG_MESSAGE << "Failed to open UPNP. Using NAT punch through." << std::endl;
    // enter_phase(SEARCHING_FOR_OTHER_INSTANCES);
    enter_phase(OPENING_UPNP);
  });

  enter_phase(CONNECTING_TO_SERVER);
}

////////////////////////////////////////////////////////////////////////////////

void Network::update() {

  http_.update();

  if (phase_ == HOSTING_INSTANCE && update_timer_.get_elapsed() > 20.0) {
    upload_game();
    update_timer_.reset();
  }

  for (RakNet::Packet* packet=peer_.peer_->Receive(); packet; peer_.peer_->DeallocatePacket(packet), packet=peer_.peer_->Receive()) {
    switch (packet->data[0]) {

      // ##################### BASIC PACKETS ###################################
      // -----------------------------------------------------------------------
      case ID_NEW_INCOMING_CONNECTION:
        // Logger::LOG_MESSAGE << "Incoming connection from " << packet->guid.ToString() << "." << std::endl;
        break;

      // -----------------------------------------------------------------------
      case ID_REMOTE_NEW_INCOMING_CONNECTION:
        // Logger::LOG_MESSAGE << "New remote incoming connection from " << packet->guid.ToString() << "." << std::endl;
        break;

      // -----------------------------------------------------------------------
      case ID_CONNECTION_REQUEST_ACCEPTED:
        if (phase_ == CONNECTING_TO_SERVER) {
          Logger::LOG_MESSAGE << "Connected to NAT server." << std::endl;
          nat_server_address_ = packet->systemAddress.ToString();
          enter_phase(SEARCHING_FOR_OTHER_INSTANCES);

        } else if (phase_ == CONNECTING_TO_HOST) {
          Logger::LOG_MESSAGE << "Connected to host " << packet->guid.ToString() << ". Sending join request." << std::endl;
          peer_.request_join(packet->guid.g);

        } else if (phase_ == HOSTING_INSTANCE) {
          Logger::LOG_MESSAGE << "Connected to client " << packet->guid.ToString() << "." << std::endl;
        }
        break;

      // -----------------------------------------------------------------------
      case ID_DISCONNECTION_NOTIFICATION:
        Logger::LOG_MESSAGE << packet->guid.ToString() << " disconnected." << std::endl;
        break;

      // ################ NAT PUNCH THROUGH PACKETS ############################
      // -----------------------------------------------------------------------
      case ID_NAT_PUNCHTHROUGH_SUCCEEDED:
        if (phase_ == CONNECTING_TO_HOST) {
          Logger::LOG_MESSAGE << "Found route to host. Connecting..." << std::endl;
          peer_.connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort());
        }
        break;

      // -----------------------------------------------------------------------
      case ID_NAT_PUNCHTHROUGH_FAILED:
        if (phase_ == CONNECTING_TO_HOST) {
          Logger::LOG_MESSAGE << "NAT punch through failed. Trying to request forwarding!" << std::endl;

          peer_.udp_proxy_->RequestForwarding(
            RakNet::SystemAddress(nat_server_address_.c_str()),
            RakNet::UNASSIGNED_SYSTEM_ADDRESS, packet->systemAddress, 7000);
        } else {
          Logger::LOG_MESSAGE << "NAT punch through failed. Ignoring it!" << std::endl;
        }
        break;

      // -----------------------------------------------------------------------
      case ID_NAT_RESPOND_BOUND_ADDRESSES:
        // ignore packet
        break;

      // ################## FULLY CONNECTED MESH ###############################
      // -----------------------------------------------------------------------
      case ID_FCM2_NEW_HOST:

        if (packet->guid.g == peer_.get_guid()) {
          if (phase_ != HOSTING_INSTANCE) {
            enter_phase(HOSTING_INSTANCE);
          }
        } else {
          Logger::LOG_MESSAGE << packet->guid.ToString() << " is host now." << std::endl;

          RakNet::BitStream bs(packet->data, packet->length, false);
          bs.IgnoreBytes(1);
          RakNet::RakNetGUID old_host;
          bs.Read(old_host);

          if (old_host != RakNet::UNASSIGNED_RAKNET_GUID) {
            Logger::LOG_MESSAGE << "Old host was " << old_host.ToString() << std::endl;
          } else {
            Logger::LOG_MESSAGE << "There was no host before." << std::endl;
          }
        }
        break;

      // -----------------------------------------------------------------------
      case (ID_USER_PACKET_ENUM + REQUEST_JOIN):
        Logger::LOG_MESSAGE << "Got join request from " << packet->guid.ToString() << "." << std::endl;
        peer_.start_join(packet->guid.g);
        break;

      // -----------------------------------------------------------------------
      case ID_FCM2_VERIFIED_JOIN_CAPABLE:
        Logger::LOG_MESSAGE << "ID_FCM2_VERIFIED_JOIN_CAPABLE." << std::endl;
        peer_.mesh_->RespondOnVerifiedJoinCapable(packet, true, 0);
        break;

      // -----------------------------------------------------------------------
      case ID_FCM2_VERIFIED_JOIN_ACCEPTED: {
        DataStructures::List<RakNet::RakNetGUID> accepted_systems;
        bool this_was_accepted;
        peer_.mesh_->GetVerifiedJoinAcceptedAdditionalData(packet, &this_was_accepted, accepted_systems, 0);
        if (this_was_accepted) {
          Logger::LOG_MESSAGE << "Join accepted." << std::endl;
        } else {
          Logger::LOG_MESSAGE << "Peer " << accepted_systems[0].ToString() << " joined the game." << std::endl;
        }

        if (this_was_accepted) {
          enter_phase(PARTICIPATING);
        }
      } break;

      // -----------------------------------------------------------------------
      case ID_FCM2_VERIFIED_JOIN_REJECTED:
        Logger::LOG_MESSAGE << "Join rejected." << std::endl;
        peer_.peer_->CloseConnection(packet->guid, true);
        break;

      // -----------------------------------------------------------------------
      case ID_FCM2_VERIFIED_JOIN_FAILED:
        Logger::LOG_MESSAGE << "Join failed." << std::endl;
        break;

      // -----------------------------------------------------------------------
      case ID_FCM2_VERIFIED_JOIN_START:
        enter_phase(CONNECTING_TO_PEERS);
        peer_.join(packet->guid.g, nat_server_address_);
        break;

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

void Network::enter_phase(Phase phase) {
  phase_ = phase;

  switch (phase) {

    // -------------------------------------------------------------------------
    case CONNECTING_TO_SERVER:
      Logger::LOG_MESSAGE << "Connecting to NAT server..." << std::endl;
      peer_.connect("natpunch.jenkinssoftware.com", 61111);
      break;

    // -------------------------------------------------------------------------
    case OPENING_UPNP:
      Logger::LOG_MESSAGE << "Opening UPNP..." << std::endl;
      upnp_.open(peer_);
      break;

    // -------------------------------------------------------------------------
    case SEARCHING_FOR_OTHER_INSTANCES:
      Logger::LOG_MESSAGE << "Downloading running instances from "
                          << "master server..." << std::endl;
      http_.get("masterserver2.raknet.com/testServer?__gameId=" + game_ID_,
                "masterserver2.raknet.com", 80);
      break;

    // -------------------------------------------------------------------------
    case CONNECTING_TO_HOST:
      Logger::LOG_MESSAGE << "Found running instance. Searching route to host "
                          << host_guid_ << "..." << std::endl;
      peer_.open_nat(host_guid_, nat_server_address_);
      break;

    // -------------------------------------------------------------------------
    case STARTING_NEW_INSTANCE:
      Logger::LOG_MESSAGE << "No running instances found. "
                          << "Starting new instance..." << std::endl;
      peer_.mesh_->ResetHostCalculation();
      upload_game();
      update_timer_.reset();
      break;

    // -------------------------------------------------------------------------
    case HOSTING_INSTANCE:
      Logger::LOG_MESSAGE << "We are host now..." << std::endl;
      upload_game();
      update_timer_.reset();
      break;

    // -------------------------------------------------------------------------
    case PARTICIPATING:
      Logger::LOG_MESSAGE << "Participating game..." << std::endl;
      break;

    // -------------------------------------------------------------------------
    case CONNECTING_TO_PEERS:

      break;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Network::upload_game() {
  std::stringstream game_descriptor;
  game_descriptor << "{'__gameId': '" << game_ID_ << "', '__clientReqId': '0', "
                  << "'__rowId': '0', '__timeoutSec': '45', "
                  << "'guid': '" << peer_.peer_->GetMyGUID().ToString() << "' }";

  http_.post("masterserver2.raknet.com/testServer", game_descriptor.str(),
             "masterserver2.raknet.com", 80);
}

////////////////////////////////////////////////////////////////////////////////

}
