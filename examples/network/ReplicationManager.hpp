////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_REPLICATION_MANAGER_HPP
#define SWIFT2D_REPLICATION_MANAGER_HPP

// includes  -------------------------------------------------------------------
#include "ReplicationConnection.hpp"

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class ReplicationManager: public RakNet::ReplicaManager3 {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  virtual RakNet::Connection_RM3* AllocConnection(RakNet::SystemAddress const& systemAddress, RakNet::RakNetGUID rakNetGUID) const {
    return new ReplicationConnection(systemAddress, rakNetGUID);
  }
  virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const {
    delete connection;
  }
};

}

#endif  // SWIFT2D_REPLICATION_MANAGER_HPP
