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
template<typename T>
class SWIFT_DLL NetworkObject : public NetworkObjectBase {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  NetworkObject(bool is_local)
    : NetworkObjectBase()
    , is_local_(is_local) {}

  static void init(std::string const& type_name) {
    if (!initialized_) {
      initialized_ = true;
      type_name_ = RakNet::RakString(type_name.c_str());
      swift::Network::get().register_type<T>(type_name_);
    }
  }

  RakNet::RakString const& get_type() const {
    return type_name_;
  };

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

  static RakNet::RakString type_name_;
  static bool              initialized_;
};

template<typename T>
RakNet::RakString NetworkObject<T>::type_name_ = "";

template<typename T>
bool NetworkObject<T>::initialized_ = false;

}

#endif  // SWIFT2D_NETWORK_OBJECT_HPP
