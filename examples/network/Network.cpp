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

#include "../../third_party/raknet/src/MessageIdentifiers.h"
#include "../../third_party/raknet/src/RakNetTypes.h"
#include <../../third_party/raknet/src/RakPeerInterface.h>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Network::Network(std::string const& game_ID)
  : game_ID_(game_ID) {

  http_.on_response.connect([&](std::string const& response){
    std::cout << "RESPONSE: " << response << std::endl;
  });

  upnp_.on_success.connect([&](){
    swift::Logger::LOG_MESSAGE << "Successfully opened UPNP!" << std::endl;
    http_.get("masterserver2.raknet.com/testServer?__gameId=" + game_ID_, "masterserver2.raknet.com", 80);
  });

  upnp_.on_fail.connect([&](){
    swift::Logger::LOG_MESSAGE << "Failed to open UPNP. Retrying..." << std::endl;
    upnp_.open(peer_);
  });

  // swift::Logger::LOG_MESSAGE << "Detecting own IP..." << std::endl;
  // peer_.connect("natpunch.jenkinssoftware.com", 61111);

  std::stringstream game_descriptor;
  game_descriptor << "{'__gameId': '" << game_ID << "', '__clientReqId': '0', "
                  << "'__rowId': '0', '__timeoutSec': '30', "
                  << "'guid': '" << peer_.peer_->GetMyGUID().ToString() << "' }";

  http_.post("masterserver2.raknet.com/testServer", game_descriptor.str(),
             "masterserver2.raknet.com", 80);

  swift::Logger::LOG_MESSAGE << "Opening UPNP..." << std::endl;
  upnp_.open(peer_);
}

////////////////////////////////////////////////////////////////////////////////

void Network::update() {

  http_.update();

//   for (RakNet::Packet* packet=peer.peer_->Receive(); packet; peer.peer_->DeallocatePacket(packet), packet=peer.peer_->Receive()) {
//     switch (packet->data[0]) {

//       case ID_CONNECTION_REQUEST_ACCEPTED:
//         if (phase == DETECT_NAT_PUNCH_ADDRESS) {
//           swift::Logger::LOG_DEBUG << "Nat punch guid is " << packet->guid.ToString() << std::endl;
//           swift::Logger::LOG_DEBUG << "Nat punch IP is "  << packet->systemAddress.ToString() << std::endl;

//           phase = OPEN_UPNP;
//           swift::Logger::LOG_MESSAGE << "Opening UPNP..." << std::endl;
//           upnp.open(peer);
//         }
//         break;

//       case ID_ADVERTISE_SYSTEM:
//         if (packet->guid!=peer->GetMyGUID())
//           peer->Connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort(),0,0);
//         break;

//       case ID_FCM2_NEW_HOST: {
//         if (packet->guid==peer->GetMyGUID())
//           printf("Got new host (ourselves)");
//         else
//           printf("Got new host %s, GUID=%s", packet->systemAddress.ToString(true), packet->guid.ToString());
//           RakNet::BitStream bs(packet->data,packet->length,false);
//           bs.IgnoreBytes(1);
//           RakNet::RakNetGUID oldHost;
//           bs.Read(oldHost);
//           // If oldHost is different then the current host, then we lost connection to the host
//           if (oldHost!=RakNet::UNASSIGNED_RAKNET_GUID)
//             printf(". Oldhost Guid=%s\n", oldHost.ToString());
//           else
//             printf(". (First reported host)\n");
//         }
//         break;

  //     default:
  //       std::cout << "Message with identifier " << packet->data[0] << " has arrived." << std::endl;
  //       break;
  //     }
  // }


  // for (int i=0; i < 32; i++)
  // {
  //   if (peer->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS,0).GetPort()!=60000+i)
  //     peer->AdvertiseSystem("255.255.255.255", 60000+i, 0,0,0);
  // }
}

////////////////////////////////////////////////////////////////////////////////

}
