////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/network/NetworkObjectBase.hpp>

#include <swift2d/utils/Logger.hpp>

#include <raknet/src/RakPeerInterface.h>
#include <raknet/src/GetTime.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::WriteAllocationID(RakNet::Connection_RM3 *destinationConnection, RakNet::BitStream *allocationIdBitstream) const {
  allocationIdBitstream->Write(get_type());
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3ConstructionState NetworkObjectBase::QueryConstruction(RakNet::Connection_RM3 *destinationConnection, RakNet::ReplicaManager3 *replicaManager3) {
  return QueryConstruction_PeerToPeer(destinationConnection);
}

////////////////////////////////////////////////////////////////////////////////

bool NetworkObjectBase::QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) {
  return QueryRemoteConstruction_PeerToPeer(sourceConnection);
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection) {
  vd_serializer_.AddRemoteSystemVariableHistory(destinationConnection->GetRakNetGUID());
  constructionBitstream->Write(get_type() + RakNet::RakString(" SerializeConstruction"));
}

////////////////////////////////////////////////////////////////////////////////

bool NetworkObjectBase::DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
  print_bitstream(constructionBitstream);
  return true;
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection) {
  vd_serializer_.RemoveRemoteSystemVariableHistory(destinationConnection->GetRakNetGUID());
  destructionBitstream->Write(get_type() + RakNet::RakString(" SerializeDestruction"));
}

////////////////////////////////////////////////////////////////////////////////

bool NetworkObjectBase::DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
  print_bitstream(destructionBitstream);
  return true;
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3ActionOnPopConnection NetworkObjectBase::QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const {
  return QueryActionOnPopConnection_PeerToPeer(droppedConnection);
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::DeallocReplica(RakNet::Connection_RM3 *sourceConnection) {
  delete this;
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3QuerySerializationResult NetworkObjectBase::QuerySerialization(RakNet::Connection_RM3 *destinationConnection) {
  return QuerySerialization_PeerToPeer(destinationConnection);
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3SerializationResult NetworkObjectBase::Serialize(RakNet::SerializeParameters *serializeParameters) {

  RakNet::VariableDeltaSerializer::SerializationContext ctx;

  serializeParameters->pro[0].reliability=RELIABLE_ORDERED;
  vd_serializer_.BeginIdenticalSerialize(
    &ctx,
    serializeParameters->whenLastSerialized==0,
    &serializeParameters->outputBitstream[0]
  );
  for (auto& member: distributed_members_) {
    member.serialize(&ctx, &vd_serializer_);
  }
  vd_serializer_.EndSerialize(&ctx);

  return RakNet::RM3SR_SERIALIZED_ALWAYS_IDENTICALLY;
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
  RakNet::VariableDeltaSerializer::DeserializationContext ctx;

  vd_serializer_.BeginDeserialize(&ctx, &deserializeParameters->serializationBitstream[0]);
  for (auto& member: distributed_members_) {
    member.deserialize(&ctx, &vd_serializer_);
  }
  vd_serializer_.EndDeserialize(&ctx);
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::OnUserReplicaPreSerializeTick(void) {
  vd_serializer_.OnPreSerializeTick();
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::distribute_member(SerializableReference const& value) {
  distributed_members_.push_back(value);
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::print_bitstream(RakNet::BitStream *bs) {
  if (bs->GetNumberOfBitsUsed() == 0) {
    return;
  }

  RakNet::RakString string;
  bs->Read(string);
  Logger::LOG_TRACE << "NetworkObjectBase receive: " << string.C_String() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

}
