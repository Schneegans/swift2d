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

#include <thread>

// main ------------------------------------------------------------------------
int main() {

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

  http.post("masterserver2.raknet.com/testServer",
            "{'__gameId': 'myTestGame', '__clientReqId': '0', '__rowId': '0', '__timeoutSec': '30' }",
            "masterserver2.raknet.com", 80);

  swift::Logger::LOG_MESSAGE << "Opening UPNP..." << std::endl;
  upnp.open(peer);

  while (true) {
  //   for (RakNet::Packet* packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive()) {
  //     switch (packet->data[0]) {

  //       case ID_CONNECTION_REQUEST_ACCEPTED:
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

  //       default:
  //         std::cout << "Message with identifier " << packet->data[0] << " has arrived." << std::endl;
  //         break;
  //       }
  //   }

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

