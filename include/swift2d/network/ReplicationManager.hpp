////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_REPLICATION_MANAGER_HPP
#define SWIFT2D_REPLICATION_MANAGER_HPP

// includes  -------------------------------------------------------------------
#include <raknet/ReplicaManager3.h>

#include <swift2d/utils/platform.hpp>

#include <functional>
#include <map>

namespace swift {

class NetworkObjectBase;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL ReplicationManager : public RakNet::ReplicaManager3 {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  /*virtual*/ RakNet::Connection_RM3* AllocConnection(RakNet::SystemAddress const& systemAddress, RakNet::RakNetGUID rakNetGUID) const;
  /*virtual*/ void                    DeallocConnection(RakNet::Connection_RM3 *connection) const;

  void register_object(RakNet::RakString const& name, std::function<NetworkObjectBase*()> const& factory);
  NetworkObjectBase* create_object(RakNet::RakString const& name) const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  std::map<RakNet::RakString, std::function<NetworkObjectBase*()>> object_registry_;
};

}

#endif  // SWIFT2D_REPLICATION_MANAGER_HPP
