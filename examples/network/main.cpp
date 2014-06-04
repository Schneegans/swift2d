////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/swift2d.hpp>


#include <stdio.h>
#include <string.h>
#include <../../third_party/raknet/src/RakPeerInterface.h>
#include <../../third_party/raknet/src/MessageIdentifiers.h>
#include <../../third_party/raknet/src/BitStream.h>
#include <../../third_party/raknet/src/RakNetTypes.h>
#include <../../third_party/raknet/src/ConnectionGraph2.h>
#include <../../third_party/raknet/src/FullyConnectedMesh2.h>
#include <../../third_party/raknet/src/RakSleep.h>
#include <../../third_party/raknet/src/HTTPConnection2.h>
#include <../../third_party/raknet/src/TCPInterface.h>


// enum GameMessages {
//   ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM+1
// };

int main() {

  auto fcm2(RakNet::FullyConnectedMesh2::GetInstance());
  fcm2->SetAutoparticipateConnections(true);

  auto cg2(RakNet::ConnectionGraph2::GetInstance());
  auto peer(RakNet::RakPeerInterface::GetInstance());
  peer->AttachPlugin(fcm2);
  peer->AttachPlugin(cg2);

  auto httpConnection2(RakNet::HTTPConnection2::GetInstance());
  auto tcp(RakNet::TCPInterface::GetInstance());
  tcp->AttachPlugin(httpConnection2);

  RakNet::SocketDescriptor sd;
  sd.socketFamily=AF_INET;
  sd.port=0;
  RakNet::StartupResult sr = peer->Startup(8,&sd,1);
  RakAssert(sr==RakNet::RAKNET_STARTED);
  peer->SetMaximumIncomingConnections(8);
  peer->SetTimeoutTime(1000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);
  printf("Our guid is %s\n", peer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
  printf("Started on %s\n", peer->GetMyBoundAddress().ToString(true));

  tcp->Start(0,0,1);

  RakNet::ConnectionAttemptResult car = peer->Connect("natpunch.jenkinssoftware.com", 61111, 0, 0);
  if (car!=RakNet::CONNECTION_ATTEMPT_STARTED) {
    printf("Failed connect call to %s. Code=%i\n", "natpunch.jenkinssoftware.com", car);
  }

  RakNet::Packet *packet;

  while (1)
  {
    for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
    {
      switch (packet->data[0])
        {
        case ID_DISCONNECTION_NOTIFICATION:
          // Connection lost normally
          printf("ID_DISCONNECTION_NOTIFICATION\n");
          break;

        case ID_NEW_INCOMING_CONNECTION:
          // Somebody connected.  We have their IP now
          printf("ID_NEW_INCOMING_CONNECTION from %s. guid=%s.\n", packet->systemAddress.ToString(true), packet->guid.ToString());
          break;

        case ID_CONNECTION_REQUEST_ACCEPTED:
          // Somebody connected.  We have their IP now
          printf("ID_CONNECTION_REQUEST_ACCEPTED from %s. guid=%s.\n", packet->systemAddress.ToString(true), packet->guid.ToString());
          break;

        case ID_CONNECTION_LOST:
          // Couldn't deliver a reliable packet - i.e. the other system was abnormally
          // terminated
          printf("ID_CONNECTION_LOST\n");
          break;

        case ID_ADVERTISE_SYSTEM:
          if (packet->guid!=peer->GetMyGUID())
            peer->Connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort(),0,0);
          break;

        case ID_FCM2_NEW_HOST:
          {
          if (packet->guid==peer->GetMyGUID())
            printf("Got new host (ourselves)");
          else
            printf("Got new host %s, GUID=%s", packet->systemAddress.ToString(true), packet->guid.ToString());
            RakNet::BitStream bs(packet->data,packet->length,false);
            bs.IgnoreBytes(1);
            RakNet::RakNetGUID oldHost;
            bs.Read(oldHost);
            // If oldHost is different then the current host, then we lost connection to the host
            if (oldHost!=RakNet::UNASSIGNED_RAKNET_GUID)
              printf(". Oldhost Guid=%s\n", oldHost.ToString());
            else
              printf(". (First reported host)\n");
          }
          break;
        default:
          printf("Message with identifier %i has arrived.\n", packet->data[0]);
          break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // The following code is TCP operations for talking to the master server,
    // and parsing the reply
    RakNet::SystemAddress sa;
    // This is kind of crappy, but for TCP plugins, always do
    // HasCompletedConnectionAttempt, then Receive(), then
    // HasFailedConnectionAttempt(), HasLostConnection()
    sa = tcp->HasCompletedConnectionAttempt();
    for (packet = tcp->Receive(); packet; tcp->DeallocatePacket(packet), packet = tcp->Receive());
    sa = tcp->HasFailedConnectionAttempt();
    sa = tcp->HasLostConnection();

    RakNet::RakString stringTransmitted;
    RakNet::RakString hostTransmitted;
    RakNet::RakString responseReceived;
    RakNet::SystemAddress hostReceived;
    int contentOffset;
    if (httpConnection2->GetResponse(stringTransmitted, hostTransmitted, responseReceived, hostReceived, contentOffset)) {
      if (!responseReceived.IsEmpty()) {
        std::cout << responseReceived << std::endl;
      }
    }

    ////////////////////////////////////////////////////////////////////////////
    RakSleep(30);
    // for (int i=0; i < 32; i++)
    // {
    //   if (peer->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS,0).GetPort()!=60000+i)
    //     peer->AdvertiseSystem("255.255.255.255", 60000+i, 0,0,0);
    // }
  }

  peer->Shutdown(100);
  RakNet::RakPeerInterface::DestroyInstance(peer);
  RakNet::ConnectionGraph2::DestroyInstance(cg2);
  RakNet::FullyConnectedMesh2::DestroyInstance(fcm2);
  RakNet::HTTPConnection2::DestroyInstance(httpConnection2);
  RakNet::TCPInterface::DestroyInstance(tcp);

  return 0;
}
