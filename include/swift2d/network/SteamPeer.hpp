////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_STEAM_PEER_HPP
#define SWIFT2D_STEAM_PEER_HPP

// includes  -------------------------------------------------------------------
#include <raknet/src/Lobby2Message_Steam.h>

#include <string>

namespace RakNet {
  class RakPeerInterface;
  class ConnectionGraph2;
  class FullyConnectedMesh2;
  class NatPunchthroughClient;
  class NatTypeDetectionClient;
  class NetworkIDManager;
}

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class ReplicationManager;

// -----------------------------------------------------------------------------
class SteamPeer : public RakNet::Lobby2Callbacks {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  SteamPeer();
  ~SteamPeer();

  uint64_t get_guid() const;

  // -------------------------------------------------- steam callback interface
  virtual void MessageResult(RakNet::Notification_Console_MemberJoinedRoom *message);
  virtual void MessageResult(RakNet::Console_SearchRooms *message);
  virtual void ExecuteDefaultResult(RakNet::Lobby2Message *message);

  friend class SteamNetwork;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  RakNet::RakPeerInterface*             peer_;
  RakNet::ConnectionGraph2*             graph_;
  RakNet::FullyConnectedMesh2*          mesh_;
  RakNet::NetworkIDManager*             id_manager_;

  RakNet::Lobby2Client_Steam*           steam_;
  RakNet::Lobby2MessageFactory_Steam*   steam_messages_;

  ReplicationManager*                   replica_;

  uint64_t last_room_;
};

}

#endif  // SWIFT2D_STEAM_PEER_HPP

