////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "Peer.hpp"

#include <swift2d/utils/Logger.hpp>

#include <../../third_party/raknet/src/ConnectionGraph2.h>
#include <../../third_party/raknet/src/FullyConnectedMesh2.h>
#include <../../third_party/raknet/src/RakPeerInterface.h>

#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Peer::Peer()
  : peer_(RakNet::RakPeerInterface::GetInstance())
  , graph_(RakNet::ConnectionGraph2::GetInstance())
  , mesh_(RakNet::FullyConnectedMesh2::GetInstance()) {

  mesh_->SetAutoparticipateConnections(true);

  peer_->AttachPlugin(mesh_);
  peer_->AttachPlugin(graph_);

  RakNet::SocketDescriptor sd;
  sd.socketFamily=AF_INET;
  sd.port=0;

  RakNet::StartupResult sr = peer_->Startup(8,&sd,1);

  if (sr != RakNet::RAKNET_STARTED) {
    Logger::LOG_ERROR << "Failed to start peer!" << std::endl;
  }

  peer_->SetMaximumIncomingConnections(8);
  peer_->SetTimeoutTime(1000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);

  Logger::LOG_DEBUG << "Our guid is " << peer_->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString() << std::endl;
  Logger::LOG_DEBUG << "Started on "  << peer_->GetMyBoundAddress().ToString(true) << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

Peer::~Peer() {
  peer_->Shutdown(100);
  RakNet::RakPeerInterface::DestroyInstance(peer_);
  RakNet::ConnectionGraph2::DestroyInstance(graph_);
  RakNet::FullyConnectedMesh2::DestroyInstance(mesh_);
}

////////////////////////////////////////////////////////////////////////////////

}
