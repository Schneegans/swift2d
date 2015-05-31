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

// includes  -------------------------------------------------------------------
#include <swift2d/network/Network.hpp>

#include <swift2d/network/NetworkObjectBase.hpp>
#include <swift2d/network/ReplicationManager.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/math/operators.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <raknet/MessageIdentifiers.h>
#include <raknet/RakNetTypes.h>
#include <raknet/RakPeerInterface.h>
#include <raknet/BitStream.h>
#include <raknet/FullyConnectedMesh2.h>
#include <raknet/PacketLogger.h>
#include <raknet/NatTypeDetectionClient.h>
#include <raknet/ConnectionGraph2.h>
#include <raknet/NatPunchthroughClient.h>
#include <raknet/NetworkIDManager.h>
#include <raknet/RPC3.h>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Network::Network()
  : phase_      (GETTING_EXTERNAL_IP)
  , peer_       (RakNet::RakPeerInterface::GetInstance())
  , npt_        (RakNet::NatPunchthroughClient::GetInstance())
  , id_manager_ (new RakNet::NetworkIDManager())
  , rpc_        (new RakNet::RPC3())
  , replica_    (new ReplicationManager())
  , internal_id_("")
  , external_id_("") {

  peer_->AttachPlugin(npt_);
  peer_->AttachPlugin(replica_);
  peer_->AttachPlugin(rpc_);

  rpc_->SetNetworkIDManager(id_manager_);
  replica_->SetNetworkIDManager(id_manager_);

  RakNet::SocketDescriptor sd;
  sd.socketFamily = AF_INET;
  sd.port = 0;

  RakNet::StartupResult sr = peer_->Startup(50, &sd, 1);

  if (sr != RakNet::RAKNET_STARTED) {
    LOG_ERROR << "Failed to start peer!" << std::endl;
  } else {
    LOG_MESSAGE << "Started peer " << get_network_id() << std::endl;
  }

  peer_->SetMaximumIncomingConnections(8);
  peer_->SetTimeoutTime(3000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

  RakNet::ConnectionAttemptResult car = peer_->Connect("natpunch.jenkinssoftware.com", 61111, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////

Network::~Network() {
  peer_->Shutdown(100);
  RakNet::RakPeerInterface::DestroyInstance(peer_);

  delete replica_;
  delete id_manager_;
}

////////////////////////////////////////////////////////////////////////////////

void Network::connect(std::string const& internal_address,
                      std::string const& external_address,
                      math::uint64 network_id) {

  if (internal_address == "" || external_address == "") {
    LOG_MESSAGE << "Cannot connect to peer!" << std::endl;
    return;
  }

  if (is_in_same_network(external_address) || internal_address == external_address) {
    connect(internal_address);
  } else {
    natpunch(network_id);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Network::connect(std::string const& other) {
  auto o(RakNet::SystemAddress(other.c_str()));

  LOG_MESSAGE << "Connecting to " << o.ToString() << " via LAN..." << std::endl;
  peer_->Connect(o.ToString(false), o.GetPort(), 0, 0);
}

////////////////////////////////////////////////////////////////////////////////

void Network::natpunch(math::uint64 uuid) {
  LOG_MESSAGE << "Connecting to " << peer_->GetSystemAddressFromGuid(RakNet::RakNetGUID(uuid)).ToString() << " via NatPunch..." << std::endl;
  npt_->OpenNAT(RakNet::RakNetGUID(uuid), RakNet::SystemAddress(nat_server_address_.c_str()));
}

////////////////////////////////////////////////////////////////////////////////

void Network::disconnect(math::uint64 uuid) {
  peer_->CloseConnection(RakNet::RakNetGUID(uuid), true);
}

////////////////////////////////////////////////////////////////////////////////

void Network::update() {

  for (RakNet::Packet* packet = peer_->Receive(); packet; peer_->DeallocatePacket(packet), packet=peer_->Receive()) {
    switch (packet->data[0]) {

      // ##################### BASIC PACKETS ###################################
      // -----------------------------------------------------------------------
      case ID_CONNECTION_REQUEST_ACCEPTED:

        if (phase_ == GETTING_EXTERNAL_IP) {

          // the nat server accepted our connection
          LOG_MESSAGE << "Connected to NAT server." << std::endl;
          nat_server_address_ = packet->systemAddress.ToString();

          // save external ip
          external_id_ = peer_->GetExternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString();
          LOG_MESSAGE << "My external IP is " << external_id_ << std::endl;

          // broadcast on LAN to get internal ip
          phase_ = GETTING_INTERNAL_IP;
          peer_->AdvertiseSystem("255.255.255.255", peer_->GetInternalID().GetPort(), 0, 0);
        } else {
          LOG_MESSAGE << "Successfully connected to " << packet->guid.ToString() << "." << std::endl;
          on_connection_result.emit(packet->systemAddress.ToString(), true);
        } 

        break;

      // -----------------------------------------------------------------------
      case ID_ADVERTISE_SYSTEM:
        if (packet->guid.g == peer_->GetMyGUID().g) {
          // save internal ip
          internal_id_ = packet->systemAddress.ToString();
          LOG_MESSAGE << "My internal IP is " << internal_id_ << std::endl;
          phase_ = READY;
        }

        break;

      // -----------------------------------------------------------------------
      case ID_DISCONNECTION_NOTIFICATION:
        LOG_MESSAGE << packet->guid.ToString() << " disconnected." << std::endl;
        break;

      // -----------------------------------------------------------------------
      case ID_CONNECTION_ATTEMPT_FAILED:
        LOG_MESSAGE << "Failed to connect to " << packet->systemAddress.ToString() << "." << std::endl;
        on_connection_result.emit(packet->systemAddress.ToString(), false);
        break;

      // ################ NAT PUNCH THROUGH PACKETS ############################
      // -----------------------------------------------------------------------
      case ID_NAT_PUNCHTHROUGH_SUCCEEDED:
        LOG_MESSAGE << "NAT punch succeeded. Connecting..." << std::endl;
        peer_->Connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort(), 0, 0);
        break;

      // -----------------------------------------------------------------------
      case ID_NAT_PUNCHTHROUGH_FAILED:
        LOG_MESSAGE << "NAT punch through failed." << std::endl;
        break;

      // ##################### TRACE PACKETS ###################################
      // -----------------------------------------------------------------------
      case ID_REPLICA_MANAGER_SERIALIZE:
      case ID_REPLICA_MANAGER_SCOPE_CHANGE:
      case ID_REPLICA_MANAGER_DOWNLOAD_STARTED:
      case ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE:
      case ID_REPLICA_MANAGER_CONSTRUCTION:
        LOG_TRACE << "Got " << RakNet::PacketLogger::BaseIDTOString(packet->data[0])
                  << " from " << packet->guid.ToString() << std::endl;
        break;

      // ##################### OTHER PACKETS ###################################
      // -----------------------------------------------------------------------
      default:
        LOG_DEBUG << "Got " << RakNet::PacketLogger::BaseIDTOString(packet->data[0])
                            << " from " << packet->guid.ToString() << std::endl;
        break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Network::distribute_object(NetworkObjectBase* object) {
  replica_->Reference(object);
}

////////////////////////////////////////////////////////////////////////////////

bool Network::is_in_same_network(std::string const& other) const {
  RakNet::SystemAddress o;
  RakNet::SystemAddress self;
  if (o.FromString(other.c_str()) && self.FromString(external_id_.c_str())) {
    if (o.EqualsExcludingPort(self)) {
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////

std::string const& Network::get_internal_address() const {
  return internal_id_;
}

////////////////////////////////////////////////////////////////////////////////

std::string const& Network::get_external_address() const {
  return external_id_;
}

////////////////////////////////////////////////////////////////////////////////

math::uint64 Network::get_network_id() const {
  return peer_->GetMyGUID().g;
}

////////////////////////////////////////////////////////////////////////////////

int Network::get_average_ping(math::uint64 to_other) const {
  return peer_->GetAveragePing(RakNet::RakNetGUID(to_other));
}

////////////////////////////////////////////////////////////////////////////////

}
