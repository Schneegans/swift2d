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
#include <swift2d/network/ReplicationConnection.hpp>

#include <functional>
#include <map>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class ReplicationManager: public RakNet::ReplicaManager3 {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  /*virtual*/ RakNet::Connection_RM3* AllocConnection(RakNet::SystemAddress const& systemAddress, RakNet::RakNetGUID rakNetGUID) const;

  /*virtual*/ void DeallocConnection(RakNet::Connection_RM3 *connection) const;

  void register_object(RakNet::RakString const& name, std::function<NetworkObject*()> const& factory);
  NetworkObject* create_object(RakNet::RakString const& name) const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  std::map<RakNet::RakString, std::function<NetworkObject*()>> object_registry_;
};

}

#endif  // SWIFT2D_REPLICATION_MANAGER_HPP
