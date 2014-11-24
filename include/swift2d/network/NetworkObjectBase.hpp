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
#include <swift2d/network/Network.hpp>
#include <swift2d/network/SerializableReference.hpp>

#include <raknet/ReplicaManager3.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL NetworkObjectBase : public RakNet::Replica3 {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  virtual void on_remote_delete() = 0;
  virtual std::string get_type_name() const = 0;

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

  template<typename Function>
  void distribute_function(std::string const& function_name, Function const& f) const {
    Network::get().distribute_function(get_type_name() + function_name, f);
  }

  template<typename ...Args>
  void call_function(std::string const& function_name, Args&& ... a) {
    Network::get().call_function(get_type_name() + function_name, GetNetworkID(), a..., true);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  std::vector<SerializableReference> distributed_members_;

  RakNet::VariableDeltaSerializer vd_serializer_;
};

}

#endif  // SWIFT2D_NETWORK_OBJECT_BASE_HPP
