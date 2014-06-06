////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "UpnpOpener.hpp"

#include "Peer.hpp"
#include <swift2d/utils/Logger.hpp>

#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/upnpcommands.h>
#include <miniupnpc/upnperrors.h>

#include <../../third_party/raknet/src/RakPeerInterface.h>
#include <../../third_party/raknet/src/GetTime.h>
#include <../../third_party/raknet/src/DS_List.h>
#include <../../third_party/raknet/src/Itoa.h>
#include <../../third_party/raknet/src/RakThread.h>

#include <iostream>
#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

namespace {

  //////////////////////////////////////////////////////////////////////////////

  struct UPNPOpenWorkerArgs {
    char            buff[256];
    unsigned short  port_to_open;
    unsigned int    timeout;
    UpnpOpener*     opener;
  };

  //////////////////////////////////////////////////////////////////////////////

  RAK_THREAD_DECLARATION(UPNPOpenWorker) {

    UPNPOpenWorkerArgs *args = ( UPNPOpenWorkerArgs * ) arguments;
    bool success=false;

    // Behind a NAT. Try to open with UPNP to avoid doing NAT punchthrough
    int error(0);
    UPNPDev* devlist = upnpDiscover(args->timeout, 0, 0, 0, 0, &error);

    if (error > 0) {
      Logger::LOG_WARNING << "upnpDiscover failed with code " << error << std::endl;

    } else if (devlist) {

      char lanaddr[64];
      UPNPUrls urls;
      IGDdatas data;

      if (UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr)) == 1) {

        freeUPNPDevlist(devlist);

        std::stringstream port_stream;
        port_stream << args->port_to_open;
        auto port(port_stream.str());

        error = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
                                    port.c_str(), port.c_str(),
                                    lanaddr, 0, "UDP", 0, "0");

        if(error != UPNPCOMMAND_SUCCESS) {
          Logger::LOG_WARNING << "AddPortMapping failed with code " << error << std::endl;

        } else {
          char int_port[6], int_client[16], desc[128], enabled[128], duration[128];
          error = UPNP_GetSpecificPortMappingEntry(urls.controlURL, data.first.servicetype,
            port.c_str(), "UDP", int_client, int_port, desc, enabled, duration);

          success = error == UPNPCOMMAND_SUCCESS;
        }
      }
    }

    if (success) {
      args->opener->on_success.emit();
    } else {
      args->opener->on_fail.emit();
    }

    RakNet::OP_DELETE(args, _FILE_AND_LINE_);
  }

  //////////////////////////////////////////////////////////////////////////////

  void UPNPOpenAsynch(unsigned short port_to_open, unsigned int timeout, UpnpOpener* opener) {

    UPNPOpenWorkerArgs *args = RakNet::OP_NEW<UPNPOpenWorkerArgs>(_FILE_AND_LINE_);
    args->port_to_open  = port_to_open;
    args->timeout       = timeout;
    args->opener        = opener;

    RakNet::RakThread::Create(UPNPOpenWorker, args);
  }

  //////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

void UpnpOpener::open(Peer const& peer) {
    DataStructures::List<RakNet::RakNetSocket2*> sockets;
    peer.peer_->GetSockets(sockets);
    UPNPOpenAsynch(sockets[0]->GetBoundAddress().GetPort(), 3000, this);
}

////////////////////////////////////////////////////////////////////////////////

}
