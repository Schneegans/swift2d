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
class NetworkObject: public NetworkObjectBase {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  static void init() {
    swift::Network::instance()->register_type<T>();
  }

  NetworkObject(std::string const& type_name) :
    type_name_(type_name.c_str()) {}

  RakNet::RakString const& get_name() const {
    return type_name_;
  };

  void distribute() {
    Network::instance()->distribute_object(this);
  }


 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  RakNet::RakString type_name_;
};

}

#endif  // SWIFT2D_NETWORK_OBJECT_HPP
