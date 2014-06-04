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


using namespace RakNet;

RakNet::RakPeerInterface *rakPeer;

int main()
{
  FullyConnectedMesh2 fcm2;
  ConnectionGraph2 cg2;
  rakPeer=RakNet::RakPeerInterface::GetInstance();
  rakPeer->AttachPlugin(&fcm2);
  rakPeer->AttachPlugin(&cg2);
  fcm2.SetAutoparticipateConnections(true);
  RakNet::SocketDescriptor sd;
  sd.socketFamily=AF_INET; // Only IPV4 supports broadcast on 255.255.255.255
  sd.port=60000;
  while (IRNS2_Berkley::IsPortInUse(sd.port, sd.hostAddress, sd.socketFamily, SOCK_DGRAM)==true)
    sd.port++;
  StartupResult sr = rakPeer->Startup(8,&sd,1);
  RakAssert(sr==RAKNET_STARTED);
  rakPeer->SetMaximumIncomingConnections(8);
  rakPeer->SetTimeoutTime(1000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);
  printf("Our guid is %s\n", rakPeer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
  printf("Started on %s\n", rakPeer->GetMyBoundAddress().ToString(true));

//  PacketLogger packetLogger;
//  rakPeer->AttachPlugin(&packetLogger);
//  packetLogger.SetLogDirectMessages(false);

  bool quit=false;
  RakNet::Packet *packet;
  char ch;
  while (!quit)
  {
    for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive())
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
        if (packet->guid!=rakPeer->GetMyGUID())
          rakPeer->Connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort(),0,0);
        break;

      case ID_FCM2_NEW_HOST:
        {
        if (packet->guid==rakPeer->GetMyGUID())
          printf("Got new host (ourselves)");
        else
          printf("Got new host %s, GUID=%s", packet->systemAddress.ToString(true), packet->guid.ToString());
          RakNet::BitStream bs(packet->data,packet->length,false);
          bs.IgnoreBytes(1);
          RakNetGUID oldHost;
          bs.Read(oldHost);
          // If oldHost is different then the current host, then we lost connection to the host
          if (oldHost!=UNASSIGNED_RAKNET_GUID)
            printf(". Oldhost Guid=%s\n", oldHost.ToString());
          else
            printf(". (First reported host)\n");
        }
        break;
      }
    }


    RakSleep(30);
    for (int i=0; i < 32; i++)
    {
      if (rakPeer->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS,0).GetPort()!=60000+i)
        rakPeer->AdvertiseSystem("255.255.255.255", 60000+i, 0,0,0);
    }
  }

  RakNet::RakPeerInterface::DestroyInstance(rakPeer);
  return 0;
}



/*
enum GameMessages {
  ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM+1
};


#define MAX_CLIENTS 10
#define SERVER_PORT 60000

int main(void)
{
  char str[512];

  RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
  bool isServer;
  RakNet::Packet *packet;

  printf("(C) or (S)erver?\n");
  gets(str);

  if ((str[0]=='c')||(str[0]=='C'))
  {
    RakNet::SocketDescriptor sd;
    peer->Startup(1,&sd, 1);
    isServer = false;
  } else {
    RakNet::SocketDescriptor sd(SERVER_PORT,0);
    peer->Startup(MAX_CLIENTS, &sd, 1);
    isServer = true;
  }

  if (isServer)
  {
    printf("Starting the server.\n");
    // We need to let the server accept incoming connections from the clients
    peer->SetMaximumIncomingConnections(MAX_CLIENTS);
  } else {
    printf("Enter server IP or hit enter for 127.0.0.1\n");
    gets(str);
    if (str[0]==0){
      strcpy(str, "127.0.0.1");
    }
    printf("Starting the client.\n");
    peer->Connect(str, SERVER_PORT, 0,0);

  }

  while (1)
  {
    for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
    {
      switch (packet->data[0])
      {
      case ID_REMOTE_DISCONNECTION_NOTIFICATION:
        printf("Another client has disconnected.\n");
        break;
      case ID_REMOTE_CONNECTION_LOST:
        printf("Another client has lost the connection.\n");
        break;
      case ID_REMOTE_NEW_INCOMING_CONNECTION:
        printf("Another client has connected.\n");
        break;
      case ID_CONNECTION_REQUEST_ACCEPTED:
        {
          printf("Our connection request has been accepted.\n");

          // Use a BitStream to write a custom user message
          // Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
          RakNet::BitStream bsOut;
          bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
          bsOut.Write("Hello world");
          peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
        }
        break;
      case ID_NEW_INCOMING_CONNECTION:
        printf("A connection is incoming.\n");
        break;
      case ID_NO_FREE_INCOMING_CONNECTIONS:
        printf("The server is full.\n");
        break;
      case ID_DISCONNECTION_NOTIFICATION:
        if (isServer){
          printf("A client has disconnected.\n");
        } else {
          printf("We have been disconnected.\n");
        }
        break;
      case ID_CONNECTION_LOST:
        if (isServer){
          printf("A client lost the connection.\n");
        } else {
          printf("Connection lost.\n");
        }
        break;

      case ID_GAME_MESSAGE_1:
        {
          RakNet::RakString rs;
          RakNet::BitStream bsIn(packet->data,packet->length,false);
          bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
          bsIn.Read(rs);
          printf("%s\n", rs.C_String());
        }
        break;

      default:
        printf("Message with identifier %i has arrived.\n", packet->data[0]);
        break;
      }
    }
  }


  RakNet::RakPeerInterface::DestroyInstance(peer);

  return 0;
}

*/


/*
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
*/
