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
#include <raknet/src/ReplicaManager3.h>

#include <swift2d/network/SerializableReference.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class NetworkObjectBase: public RakNet::Replica3 {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  virtual RakNet::RakString const& get_type() const = 0;

  virtual void WriteAllocationID(RakNet::Connection_RM3 *destinationConnection, RakNet::BitStream *allocationIdBitstream) const;
  virtual RakNet::RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, RakNet::ReplicaManager3 *replicaManager3);
  virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection);
  virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection);
  virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection);
  virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection);
  virtual bool DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection);
  virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const;
  virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection);
  virtual RakNet::RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection);
  virtual RakNet::RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters);
  virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
  virtual void OnUserReplicaPreSerializeTick();

  void distribute_member(SerializableReference const& value);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void print_bitstream(RakNet::BitStream *bs);

  std::vector<SerializableReference> distributed_members_;

  RakNet::VariableDeltaSerializer vd_serializer_;
};

}

#endif  // SWIFT2D_NETWORK_OBJECT_BASE_HPP
