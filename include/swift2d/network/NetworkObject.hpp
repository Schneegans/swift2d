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
#include <swift2d/network/Network.hpp>
#include <swift2d/network/NetworkObjectBase.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL NetworkObject : public NetworkObjectBase {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  UInt64 OwnerID;

  NetworkObject(swift::math::uint64 owner_id, bool is_local)
    : NetworkObjectBase()
    , OwnerID(owner_id)
    , is_local_(is_local) {

    distribute_member(&OwnerID);
  }

  virtual ~NetworkObject() {
    undistribute();
  }

  template<typename T>
  static void init() {
    Network::get().register_type<T>(T::get_type_name_static());
  }

  bool is_local() const {
    return is_local_;
  }

  void distribute() {
    if (is_local_) Network::get().distribute_object(this);
  }

  void undistribute() {
    if (is_local_) BroadcastDestruction();
  }


 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  bool is_local_;
};

}

#endif  // SWIFT2D_NETWORK_OBJECT_HPP
