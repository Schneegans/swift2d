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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void Network::connect(std::string const& game_ID) {
  game_ID_ = game_ID;

  http_.on_response.connect([&](std::string const& response) {
    switch (phase_) {
      case SEARCHING_FOR_OTHER_INSTANCES: {
        Logger::LOG_MESSAGE << "Got response from master server!" << std::endl;

        bool found_other_game(false);

        try {
          boost::property_tree::ptree tree;
          std::istringstream is(response);
          boost::property_tree::read_json(is, tree);

          if (tree.get_child("GET").size() > 0) {
            auto remote_guid(tree.get_child("GET").front().second.get<std::string>("guid"));
            found_other_game = true;
          }

        } catch(std::runtime_error const& e) {
          Logger::LOG_WARNING << "Failed to parse response: " << e.what() << std::endl;
        }

        if (found_other_game) enter_phase(CONNECT_TO_RUNNING_INSTANCE);
        else                  enter_phase(START_NEW_INSTANCE);

        } break;

      case START_NEW_INSTANCE:
        Logger::LOG_MESSAGE << "Successfully registered new instance." << std::endl;
        enter_phase(HOSTING_INSTANCE);
        break;

      default:
        Logger::LOG_WARNING << "Got unexpected HTTP response." << std::endl;
        break;
    }
  });

  upnp_.on_success.connect([=](){
    Logger::LOG_MESSAGE << "Successfully opened UPNP!" << std::endl;
    enter_phase(SEARCHING_FOR_OTHER_INSTANCES);
  });

  upnp_.on_fail.connect([&](){
    Logger::LOG_MESSAGE << "Failed to open UPNP. Retrying..." << std::endl;
    enter_phase(OPEN_UPNP);
  });

  enter_phase(SEARCHING_FOR_OTHER_INSTANCES);
  // enter_phase(OPEN_UPNP);
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

void Network::enter_phase(Phase phase) {
  phase_ = phase;

  switch (phase) {
    case OPEN_UPNP:
      Logger::LOG_MESSAGE << "Opening UPNP..." << std::endl;
      upnp_.open(peer_);
      break;

    case SEARCHING_FOR_OTHER_INSTANCES:
      Logger::LOG_MESSAGE << "Downloading running instances from master server..." << std::endl;
      http_.get("masterserver2.raknet.com/testServer?__gameId=" + game_ID_, "masterserver2.raknet.com", 80);
      break;

    case CONNECT_TO_RUNNING_INSTANCE:
      Logger::LOG_MESSAGE << "Found running instance. Connecting to remote host..." << std::endl;
      break;

    case START_NEW_INSTANCE: {
      Logger::LOG_MESSAGE << "No running instances found. Starting new instance..." << std::endl;
      std::stringstream game_descriptor;
      game_descriptor << "{'__gameId': '" << game_ID_ << "', '__clientReqId': '0', "
                      << "'__rowId': '0', '__timeoutSec': '30', "
                      << "'guid': '" << peer_.peer_->GetMyGUID().ToString() << "' }";

      http_.post("masterserver2.raknet.com/testServer", game_descriptor.str(),
                 "masterserver2.raknet.com", 80);
      } break;

    case HOSTING_INSTANCE:
      Logger::LOG_MESSAGE << "Starting game..." << std::endl;
      host_ = true;
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////

}
