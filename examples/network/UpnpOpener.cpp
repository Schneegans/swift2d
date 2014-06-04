////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "UpnpOpener.hpp"

#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/upnpcommands.h>
#include <miniupnpc/upnperrors.h>

#include <../../third_party/raknet/src/RakPeerInterface.h>
#include <../../third_party/raknet/src/GetTime.h>
#include <../../third_party/raknet/src/DS_List.h>
#include <../../third_party/raknet/src/Itoa.h>
#include <../../third_party/raknet/src/RakThread.h>

#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

namespace {

  struct UPNPOpenWorkerArgs {
    char            buff[256];
    unsigned short  portToOpen;
    unsigned int    timeout;
    UpnpOpener*     opener;
    void (*resultCallback)(bool success, unsigned short portToOpen, UpnpOpener* opener);
    void (*progressCallback)(const char *progressMsg, UpnpOpener* opener);
  };

  RAK_THREAD_DECLARATION(UPNPOpenWorker) {

    UPNPOpenWorkerArgs *args = ( UPNPOpenWorkerArgs * ) arguments;
    bool success=false;

    // Behind a NAT. Try to open with UPNP to avoid doing NAT punchthrough
    struct UPNPDev * devlist = 0;
    RakNet::Time t1 = RakNet::GetTime();
    devlist = upnpDiscover(args->timeout, 0, 0, 0, 0, 0);
    RakNet::Time t2 = RakNet::GetTime();
    if (devlist) {
      if (args->progressCallback)
        args->progressCallback("List of UPNP devices found on the network :\n", args->opener);
      struct UPNPDev * device;
      for(device = devlist; device; device = device->pNext) {
        sprintf(args->buff, " desc: %s\n st: %s\n\n", device->descURL, device->st);
        if (args->progressCallback) {
          args->progressCallback(args->buff, args->opener);
        }
      }

      char lanaddr[64]; /* my ip address on the LAN */
      struct UPNPUrls urls;
      struct IGDdatas data;
      if (UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr))==1) {
        char iport[32];
        Itoa(args->portToOpen, iport,10);
        char eport[32];
        strcpy(eport, iport);

        // Version miniupnpc-1.6.20120410
        int r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
          eport, iport, lanaddr, 0, "UDP", 0, "0");

        if(r!=UPNPCOMMAND_SUCCESS) {
          printf("AddPortMapping(%s, %s, %s) failed with code %d (%s)\n",
          eport, iport, lanaddr, r, strupnperror(r));
        }

        char intPort[6];
        char intClient[16];

        // Version miniupnpc-1.6.20120410
        char desc[128];
        char enabled[128];
        char leaseDuration[128];
        r = UPNP_GetSpecificPortMappingEntry(urls.controlURL,
          data.first.servicetype,
          eport, "UDP",
          intClient, intPort,
          desc, enabled, leaseDuration);

        if(r!=UPNPCOMMAND_SUCCESS) {
          sprintf(args->buff, "GetSpecificPortMappingEntry() failed with code %d (%s)\n",
            r, strupnperror(r));
          if (args->progressCallback)
            args->progressCallback(args->buff, args->opener);
        } else {
          if (args->progressCallback) {
            args->progressCallback("UPNP success.\n", args->opener);
          }
          success=true;
        }
      }
    }

    if (args->resultCallback) {
      args->resultCallback(success, args->portToOpen, args->opener);
    }
    RakNet::OP_DELETE(args, _FILE_AND_LINE_);
    return nullptr;
  }

  void UPNPOpenAsynch(unsigned short portToOpen,
      unsigned int timeout,
      void (*progressCallback)(const char *progressMsg, UpnpOpener* opener),
      void (*resultCallback)(bool success, unsigned short portToOpen, UpnpOpener* opener),
      UpnpOpener* opener) {

    UPNPOpenWorkerArgs *args = RakNet::OP_NEW<UPNPOpenWorkerArgs>(_FILE_AND_LINE_);
    args->portToOpen = portToOpen;
    args->timeout = timeout;
    args->opener = opener;
    args->progressCallback = progressCallback;
    args->resultCallback = resultCallback;

    RakNet::RakThread::Create(UPNPOpenWorker, args);
  }

  void UPNPProgressCallback(const char *progressMsg, UpnpOpener* opener) {
    std::cout << progressMsg << std::endl;
  }

  void UPNPResultCallback(bool success, unsigned short portToOpen, UpnpOpener* opener) {
    if (success) {
      std::cout << "Discovering UPNP: Success!" << std::endl;
      opener->on_success.emit();
    } else {
      std::cout << "Discovering UPNP: Fail!" << std::endl;
    }
  }

}

////////////////////////////////////////////////////////////////////////////////

void UpnpOpener::open(RakNet::RakPeerInterface* peer) {
    std::cout << "Discovering UPNP..." << std::endl;

    DataStructures::List<RakNet::RakNetSocket2*> sockets;
    peer->GetSockets(sockets);

    UPNPOpenAsynch(sockets[0]->GetBoundAddress().GetPort(), 2000, UPNPProgressCallback, UPNPResultCallback, this);
}

////////////////////////////////////////////////////////////////////////////////

}
