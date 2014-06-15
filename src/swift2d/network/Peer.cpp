////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/network/Peer.hpp>

#include <swift2d/network/Network.hpp>
#include <swift2d/network/ReplicationManager.hpp>
#include <swift2d/utils/Logger.hpp>

#include <raknet/src/ConnectionGraph2.h>
#include <raknet/src/FullyConnectedMesh2.h>
#include <raknet/src/RakPeerInterface.h>
#include <raknet/src/NatPunchthroughClient.h>
#include <raknet/src/NatTypeDetectionClient.h>
#include <raknet/src/BitStream.h>
#include <raknet/src/MessageIdentifiers.h>
#include <raknet/src/NetworkIDManager.h>

#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Peer::Peer()
  : peer_(RakNet::RakPeerInterface::GetInstance())
  , graph_(RakNet::ConnectionGraph2::GetInstance())
  , mesh_(RakNet::FullyConnectedMesh2::GetInstance())
  , npt_(RakNet::NatPunchthroughClient::GetInstance())
  , nat_type_detector_(RakNet::NatTypeDetectionClient::GetInstance())
  , id_manager_(new RakNet::NetworkIDManager())
  , replica_(new ReplicationManager()) {

  peer_->AttachPlugin(mesh_);
  peer_->AttachPlugin(graph_);
  peer_->AttachPlugin(npt_);
  peer_->AttachPlugin(nat_type_detector_);
  peer_->AttachPlugin(replica_);

  replica_->SetNetworkIDManager(id_manager_);
  replica_->SetAutoManageConnections(false,true);

  mesh_->SetAutoparticipateConnections(false);
  mesh_->SetConnectOnNewRemoteConnection(false, "");

  RakNet::SocketDescriptor sd;
  sd.socketFamily=AF_INET;
  sd.port=0;

  RakNet::StartupResult sr = peer_->Startup(8,&sd,1);

  if (sr != RakNet::RAKNET_STARTED) {
    Logger::LOG_ERROR << "Failed to start peer!" << std::endl;
  }

  peer_->SetMaximumIncomingConnections(8);
  peer_->SetTimeoutTime(1000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
}

////////////////////////////////////////////////////////////////////////////////

Peer::~Peer() {
  peer_->Shutdown(100);
  RakNet::RakPeerInterface::DestroyInstance(peer_);
  RakNet::ConnectionGraph2::DestroyInstance(graph_);
  RakNet::FullyConnectedMesh2::DestroyInstance(mesh_);

  delete replica_;
  delete id_manager_;
}

////////////////////////////////////////////////////////////////////////////////

void Peer::connect(std::string const& ip, unsigned short port) {
  RakNet::ConnectionAttemptResult car = peer_->Connect(ip.c_str(), port, 0, 0);
  if (car!=RakNet::CONNECTION_ATTEMPT_STARTED) {
    Logger::LOG_WARNING << "Failed connect to "<< ip << ". Code=" << car << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Peer::open_nat(uint64_t guid, std::string const& nat_server) {
  npt_->OpenNAT(RakNet::RakNetGUID(guid), RakNet::SystemAddress(nat_server.c_str()));
}

////////////////////////////////////////////////////////////////////////////////

void Peer::request_join(uint64_t guid) {
  mesh_->ResetHostCalculation();

  RakNet::BitStream message;
  message.Write((RakNet::MessageID)(ID_USER_PACKET_ENUM + Network::REQUEST_JOIN));
  peer_->Send(&message, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::RakNetGUID(guid), false);
}

////////////////////////////////////////////////////////////////////////////////

void Peer::start_join(uint64_t guid) {
  mesh_->StartVerifiedJoin(RakNet::RakNetGUID(guid));
}

////////////////////////////////////////////////////////////////////////////////

void Peer::join(uint64_t guid, std::string const& nat_server) {
  DataStructures::List<RakNet::SystemAddress> addresses;
  DataStructures::List<RakNet::RakNetGUID> guids;
  mesh_->GetVerifiedJoinRequiredProcessingList(RakNet::RakNetGUID(guid), addresses, guids);
  for (unsigned int i=0; i < guids.Size(); i++) {
    // if (guids[i].g != get_guid()) {
      npt_->OpenNAT(guids[i], RakNet::SystemAddress(nat_server.c_str()));
    // }
  }
}

////////////////////////////////////////////////////////////////////////////////

uint64_t Peer::get_guid() const {
  return peer_->GetMyGUID().g;
}

////////////////////////////////////////////////////////////////////////////////

}
