////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/swift2d.hpp>

#include "Peer.hpp"
#include "UpnpOpener.hpp"
#include "HttpConnection.hpp"

#include "../../third_party/raknet/src/MessageIdentifiers.h"
#include "../../third_party/raknet/src/RakNetTypes.h"
#include <../../third_party/raknet/src/RakPeerInterface.h>

#include <thread>

// enum Phase {
//   DETECT_NAT_PUNCH_ADDRESS,
//   OPEN_UPNP,
//   CONNECT_TO_PEER
// };

// main ------------------------------------------------------------------------
int main() {

  // Phase phase(DETECT_NAT_PUNCH_ADDRESS);

  swift::Peer peer;

  swift::HttpConnection http;
  http.on_response.connect([&](std::string const& response){
    std::cout << "RESPONSE: " << response << std::endl;
  });

  swift::UpnpOpener upnp;
  upnp.on_success.connect([&](){
    swift::Logger::LOG_MESSAGE << "Successfully opened UPNP!" << std::endl;
    http.get("masterserver2.raknet.com/testServer?__gameId=myTestGame", "masterserver2.raknet.com", 80);
  });

  upnp.on_fail.connect([&](){
    swift::Logger::LOG_MESSAGE << "Failed to open UPNP. Retrying..." << std::endl;
    upnp.open(peer);
  });

  // swift::Logger::LOG_MESSAGE << "Detecting own IP..." << std::endl;
  // peer.connect("natpunch.jenkinssoftware.com", 61111);

  std::stringstream game_descriptor;
  game_descriptor << "{'__gameId': 'myTestGame', '__clientReqId': '0', "
                  << "'__rowId': '0', '__timeoutSec': '30', "
                  << "'guid': '" << peer.peer_->GetMyGUID().ToString() << "' }";

  http.post("masterserver2.raknet.com/testServer", game_descriptor.str(),
            "masterserver2.raknet.com", 80);

  swift::Logger::LOG_MESSAGE << "Opening UPNP..." << std::endl;
  upnp.open(peer);

  while (true) {
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

    http.update();

    // for (int i=0; i < 32; i++)
    // {
    //   if (peer->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS,0).GetPort()!=60000+i)
    //     peer->AdvertiseSystem("255.255.255.255", 60000+i, 0,0,0);
    // }

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  return 0;
}

