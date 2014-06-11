////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "NetworkObject.hpp"

#include <swift2d/utils/Logger.hpp>

#include <../../third_party/raknet/src/RakPeerInterface.h>
#include <../../third_party/raknet/src/GetTime.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

NetworkObject::NetworkObject() {
  Logger::LOG_WARNING << "NetworkObject created!" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObject::WriteAllocationID(RakNet::Connection_RM3 *destinationConnection, RakNet::BitStream *allocationIdBitstream) const {
  allocationIdBitstream->Write(get_name());
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3ConstructionState NetworkObject::QueryConstruction(RakNet::Connection_RM3 *destinationConnection, RakNet::ReplicaManager3 *replicaManager3) {
  return QueryConstruction_PeerToPeer(destinationConnection);
}

////////////////////////////////////////////////////////////////////////////////

bool NetworkObject::QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) {
  return QueryRemoteConstruction_PeerToPeer(sourceConnection);
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObject::SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection) {
  vd_serializer_.AddRemoteSystemVariableHistory(destinationConnection->GetRakNetGUID());
  constructionBitstream->Write(get_name() + RakNet::RakString(" SerializeConstruction"));
}

////////////////////////////////////////////////////////////////////////////////

bool NetworkObject::DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
  print_bitstream(constructionBitstream);
  return true;
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObject::SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection) {
  vd_serializer_.RemoveRemoteSystemVariableHistory(destinationConnection->GetRakNetGUID());
  destructionBitstream->Write(get_name() + RakNet::RakString(" SerializeDestruction"));
}

////////////////////////////////////////////////////////////////////////////////

bool NetworkObject::DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
  print_bitstream(destructionBitstream);
  return true;
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3ActionOnPopConnection NetworkObject::QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const {
  return QueryActionOnPopConnection_PeerToPeer(droppedConnection);
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObject::DeallocReplica(RakNet::Connection_RM3 *sourceConnection) {
  delete this;
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3QuerySerializationResult NetworkObject::QuerySerialization(RakNet::Connection_RM3 *destinationConnection) {
  return QuerySerialization_PeerToPeer(destinationConnection);
}

////////////////////////////////////////////////////////////////////////////////

RakNet::RM3SerializationResult NetworkObject::Serialize(RakNet::SerializeParameters *serializeParameters) {
  RakNet::VariableDeltaSerializer::SerializationContext ctx;

  serializeParameters->pro[0].reliability=UNRELIABLE_WITH_ACK_RECEIPT;
  serializeParameters->pro[0].sendReceipt=replicaManager->GetRakPeerInterface()->IncrementNextSendReceipt();
  serializeParameters->messageTimestamp=RakNet::GetTime();

  vd_serializer_.BeginUnreliableAckedSerialize(
    &ctx,
    serializeParameters->destinationConnection->GetRakNetGUID(),
    &serializeParameters->outputBitstream[0],
    serializeParameters->pro[0].sendReceipt
    );
  vd_serializer_.SerializeVariable(&ctx, test_var_1);
  vd_serializer_.EndSerialize(&ctx);

  serializeParameters->pro[1].reliability=RELIABLE_ORDERED;
  vd_serializer_.BeginIdenticalSerialize(
    &ctx,
    serializeParameters->whenLastSerialized==0,
    &serializeParameters->outputBitstream[1]
    );
  vd_serializer_.SerializeVariable(&ctx, test_var_2);
  vd_serializer_.EndSerialize(&ctx);

  return RakNet::RM3SR_SERIALIZED_ALWAYS;
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObject::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
  RakNet::VariableDeltaSerializer::DeserializationContext ctx;

  vd_serializer_.BeginDeserialize(&ctx, &deserializeParameters->serializationBitstream[0]);
  if (vd_serializer_.DeserializeVariable(&ctx, test_var_1))
    Logger::LOG_DEBUG << "test_var_1 changed to " << test_var_1 << std::endl;
  vd_serializer_.EndDeserialize(&ctx);

  vd_serializer_.BeginDeserialize(&ctx, &deserializeParameters->serializationBitstream[1]);
  if (vd_serializer_.DeserializeVariable(&ctx, test_var_2))
    Logger::LOG_DEBUG << "test_var_2 changed to " << test_var_2 << std::endl;
  vd_serializer_.EndDeserialize(&ctx);
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObject::OnUserReplicaPreSerializeTick(void) {
  vd_serializer_.OnPreSerializeTick();
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObject::NotifyReplicaOfMessageDeliveryStatus(RakNet::RakNetGUID guid, uint32_t receiptId, bool messageArrived) {
  vd_serializer_.OnMessageReceipt(guid, receiptId, messageArrived);
}

////////////////////////////////////////////////////////////////////////////////

void NetworkObject::print_bitstream(RakNet::BitStream *bs) {
  if (bs->GetNumberOfBitsUsed() == 0) {
    return;
  }

  RakNet::RakString string;
  bs->Read(string);
  Logger::LOG_DEBUG << "NetworkObject receive: " << string.C_String() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

}
