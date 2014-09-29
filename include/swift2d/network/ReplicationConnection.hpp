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
#include <swift2d/network/NetworkObjectBase.hpp>

#include <raknet/ReplicaManager3.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class ReplicationManager;

// -----------------------------------------------------------------------------
class ReplicationConnection: public RakNet::Connection_RM3 {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  ReplicationConnection(RakNet::SystemAddress const& systemAddress,
                        RakNet::RakNetGUID guid, ReplicationManager const* parent);

  bool QueryGroupDownloadMessages() const;

  virtual RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId,
                                         RakNet::ReplicaManager3 *replicaManager3);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ReplicationManager const* parent_;
};

}

#endif  // SWIFT2D_REPLICATION_CONNECTION_HPP
