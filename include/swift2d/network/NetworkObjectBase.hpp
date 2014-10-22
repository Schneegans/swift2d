////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_NETWORK_OBJECT_BASE_HPP
#define SWIFT2D_NETWORK_OBJECT_BASE_HPP

// includes  -------------------------------------------------------------------
#include <raknet/ReplicaManager3.h>

#include <swift2d/network/SerializableReference.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL NetworkObjectBase : public RakNet::Replica3 {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  virtual RakNet::RakString const& get_type() const = 0;
  virtual void on_remote_delete() = 0;

  virtual void WriteAllocationID(RakNet::Connection_RM3 *con, RakNet::BitStream* stream) const;
  virtual RakNet::RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *con, RakNet::ReplicaManager3 *replicaManager3);
  virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *con);
  virtual void SerializeConstruction(RakNet::BitStream* stream, RakNet::Connection_RM3 *con);
  virtual bool DeserializeConstruction(RakNet::BitStream* stream, RakNet::Connection_RM3 *con);
  virtual void SerializeDestruction(RakNet::BitStream* stream, RakNet::Connection_RM3 *con);
  virtual bool DeserializeDestruction(RakNet::BitStream* stream, RakNet::Connection_RM3 *con);
  virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *con) const;
  virtual void DeallocReplica(RakNet::Connection_RM3 *con);
  virtual RakNet::RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *con);
  virtual RakNet::RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters);
  virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
  virtual void OnUserReplicaPreSerializeTick();

  void distribute_member(SerializableReference const& value);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  std::vector<SerializableReference> distributed_members_;

  RakNet::VariableDeltaSerializer vd_serializer_;
};

}

#endif  // SWIFT2D_NETWORK_OBJECT_BASE_HPP
