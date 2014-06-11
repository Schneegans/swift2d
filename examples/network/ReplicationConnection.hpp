////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_REPLICATION_CONNECTION_HPP
#define SWIFT2D_REPLICATION_CONNECTION_HPP

// includes  -------------------------------------------------------------------
#include "NetworkObject.hpp"

#include <../../third_party/raknet/src/ReplicaManager3.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class ReplicationConnection: public RakNet::Connection_RM3 {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  ReplicationConnection(RakNet::SystemAddress const& systemAddress, RakNet::RakNetGUID guid)
    : RakNet::Connection_RM3(systemAddress, guid) {}

  virtual ~ReplicationConnection() {}

  bool QueryGroupDownloadMessages() const {
    return true;
  }

  virtual RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId, RakNet::ReplicaManager3 *replicaManager3) {
    RakNet::RakString type;
    allocationId->Read(type);
    if (type=="Test") {
      return new NetworkObject();
    }
    return 0;
  }
};

}

#endif  // SWIFT2D_REPLICATION_CONNECTION_HPP
