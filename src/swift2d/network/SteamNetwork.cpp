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

#include <raknet/src/MessageIdentifiers.h>
#include <raknet/src/RakNetTypes.h>
#include <raknet/src/RakPeerInterface.h>
#include <raknet/src/BitStream.h>
#include <raknet/src/FullyConnectedMesh2.h>
#include <raknet/src/PacketLogger.h>

#include <sstream>


namespace swift {

////////////////////////////////////////////////////////////////////////////////

SteamNetwork::SteamNetwork()
  : phase_(SEARCHING_FOR_OTHER_INSTANCES) {

  update_timer_.start();

  Logger::LOG_MESSAGE << "I'm " << peer_.get_guid() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void SteamNetwork::disconnect() {
  if (phase_ == HOSTING_INSTANCE) {
    Logger::LOG_MESSAGE << "Unregistering game." << std::endl;
    // unregister_game();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SteamNetwork::connect(std::string const& game_ID) {
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

  enter_phase(CONNECTING_TO_SERVER);
}

////////////////////////////////////////////////////////////////////////////////

void SteamNetwork::update() {

  auto register_new_peer = [&](RakNet::RakNetGUID guid){
    RakNet::Connection_RM3 *connection = peer_.replica_->AllocConnection(peer_.peer_->GetSystemAddressFromGuid(guid), guid);
    if (peer_.replica_->PushConnection(connection) == false) {
      peer_.replica_->DeallocConnection(connection);
    }
  };

  http_.update();

  if (phase_ == HOSTING_INSTANCE && update_timer_.get_elapsed() > 20.0) {
    // register_game();
    update_timer_.reset();
  }

  for (RakNet::Packet* packet=peer_.peer_->Receive(); packet; peer_.peer_->DeallocatePacket(packet), packet=peer_.peer_->Receive()) {
    switch (packet->data[0]) {

      // ##################### BASIC PACKETS ###################################
      // -----------------------------------------------------------------------
      case ID_CONNECTION_REQUEST_ACCEPTED:
        if (phase_ == CONNECTING_TO_SERVER) {

          enter_phase(SEARCHING_FOR_OTHER_INSTANCES);


        } else if (phase_ == CONNECTING_TO_HOST) {
          Logger::LOG_MESSAGE << "Connected to host " << packet->guid.ToString() << ". Sending join request." << std::endl;
          // peer_.request_join(packet->guid.g);

        } else if (phase_ == HOSTING_INSTANCE) {
          Logger::LOG_MESSAGE << "Connected to client " << packet->guid.ToString() << "." << std::endl;
        }
        break;

      // -----------------------------------------------------------------------
      case ID_DISCONNECTION_NOTIFICATION:
        Logger::LOG_MESSAGE << packet->guid.ToString() << " disconnected." << std::endl;
        break;

      // ################## FULLY CONNECTED MESH ###############################
      // -----------------------------------------------------------------------
      case ID_FCM2_NEW_HOST: {
        RakNet::BitStream bs(packet->data, packet->length, false);
        bs.IgnoreBytes(1);
        RakNet::RakNetGUID old_host;
        bs.Read(old_host);

        if (packet->guid.g == peer_.get_guid()) {
          if (phase_ != HOSTING_INSTANCE) {
            enter_phase(HOSTING_INSTANCE);
          }
        } else {
          Logger::LOG_MESSAGE << packet->guid.ToString() << " is host now." << std::endl;


          if (old_host != RakNet::UNASSIGNED_RAKNET_GUID) {
            Logger::LOG_MESSAGE << "Old host was " << old_host.ToString() << std::endl;
          } else {
            Logger::LOG_MESSAGE << "There was no host before." << std::endl;
          }
        }

        if (old_host == RakNet::UNASSIGNED_RAKNET_GUID) {
          DataStructures::List<RakNet::RakNetGUID> peers;
          peer_.mesh_->GetParticipantList(peers);
          for (unsigned int i=0; i < peers.Size(); i++) {
            register_new_peer(peers[i]);
          }
        }

        } break;

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

void SteamNetwork::distribute_object(NetworkObjectBase* object) {
  peer_.replica_->Reference(object);
}

////////////////////////////////////////////////////////////////////////////////

bool SteamNetwork::is_host() const {
  return peer_.mesh_->IsConnectedHost();
}

////////////////////////////////////////////////////////////////////////////////

void SteamNetwork::enter_phase(Phase phase) {
  phase_ = phase;

  switch (phase) {

    // -------------------------------------------------------------------------
    case CONNECTING_TO_SERVER:
      Logger::LOG_MESSAGE << "Connecting to NAT server..." << std::endl;
      // peer_.connect("natpunch.jenkinssoftware.com", 61111);
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
      // peer_.open_nat(host_guid_, nat_server_address_);
      break;

    // -------------------------------------------------------------------------
    case STARTING_NEW_INSTANCE:
      Logger::LOG_MESSAGE << "No running instances found. "
                          << "Starting new instance..." << std::endl;
      peer_.mesh_->ResetHostCalculation();
      // register_game();
      update_timer_.reset();
      break;

    // -------------------------------------------------------------------------
    case HOSTING_INSTANCE:
      Logger::LOG_MESSAGE << "I'm host now." << std::endl;
      // register_game();
      update_timer_.reset();
      break;

    // -------------------------------------------------------------------------
    case PARTICIPATING:
      Logger::LOG_MESSAGE << "Participating game." << std::endl;
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////

}

