////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/network/NetworkObjectBase.hpp>

#include <swift2d/utils/Logger.hpp>

#include <raknet/RakPeerInterface.h>
#include <raknet/GetTime.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::WriteAllocationID(RakNet::Connection_RM3 *con, RakNet::BitStream* stream) const {
  stream->Write(RakNet::RakString(get_type_name().c_str()));
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3ConstructionState NetworkObjectBase::QueryConstruction(RakNet::Connection_RM3 *con, RakNet::ReplicaManager3 *replicaManager3) {
  return QueryConstruction_PeerToPeer(con);
}

////////////////////////////////////////////////////////////////////////////////

bool NetworkObjectBase::QueryRemoteConstruction(RakNet::Connection_RM3 *con) {
  return QueryRemoteConstruction_PeerToPeer(con);
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::SerializeConstruction(RakNet::BitStream* stream, RakNet::Connection_RM3 *con) {
  vd_serializer_.AddRemoteSystemVariableHistory(con->GetRakNetGUID());

  for (auto& member: distributed_members_) {
    member.serialize(stream);
  }
}

////////////////////////////////////////////////////////////////////////////////

bool NetworkObjectBase::DeserializeConstruction(RakNet::BitStream* stream, RakNet::Connection_RM3 *con) {
  for (auto& member: distributed_members_) {
    member.deserialize(stream);
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::SerializeDestruction(RakNet::BitStream* stream, RakNet::Connection_RM3 *con) {
  vd_serializer_.RemoveRemoteSystemVariableHistory(con->GetRakNetGUID());

  for (auto& member: distributed_members_) {
    member.serialize(stream);
  }
}

////////////////////////////////////////////////////////////////////////////////

bool NetworkObjectBase::DeserializeDestruction(RakNet::BitStream* stream, RakNet::Connection_RM3 *con) {
  for (auto& member: distributed_members_) {
    member.deserialize(stream);
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3ActionOnPopConnection NetworkObjectBase::QueryActionOnPopConnection(RakNet::Connection_RM3 *con) const {
  return QueryActionOnPopConnection_PeerToPeer(con);
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObjectBase::DeallocReplica(RakNet::Connection_RM3 *con) {
  on_remote_delete();
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3QuerySerializationResult NetworkObjectBase::QuerySerialization(RakNet::Connection_RM3 *con) {
  return QuerySerialization_PeerToPeer(con);
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

}
