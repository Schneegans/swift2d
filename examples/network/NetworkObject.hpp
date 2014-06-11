////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_NETWORK_OBJECT_HPP
#define SWIFT2D_NETWORK_OBJECT_HPP

// includes  -------------------------------------------------------------------
#include <../../third_party/raknet/src/ReplicaManager3.h>
#include <../../third_party/raknet/src/VariableDeltaSerializer.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class NetworkObject: public RakNet::Replica3 {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  virtual RakNet::RakString get_name() const { return RakNet::RakString("Test"); };

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
  void NotifyReplicaOfMessageDeliveryStatus(RakNet::RakNetGUID guid, uint32_t receiptId, bool messageArrived);

  int test_var_1;
  int test_var_2;
 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  void print_bitstream(RakNet::BitStream *bs);

  RakNet::VariableDeltaSerializer vd_serializer_;
};

}

#endif  // SWIFT2D_NETWORK_OBJECT_HPP
