////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_NETWORK_POSITION_UPDATE_COMPONENT_HPP
#define SWIFT2D_NETWORK_POSITION_UPDATE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/events/Scheduler.hpp>
#include <swift2d/physics/DynamicBodyComponent.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class NetworkPositionUpdateComponent;
typedef std::shared_ptr<NetworkPositionUpdateComponent>       NetworkPositionUpdateComponentPtr;
typedef std::shared_ptr<const NetworkPositionUpdateComponent> ConstNetworkPositionUpdateComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL NetworkPositionUpdateComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Vec3 PosRotUpdate;
  Vec3 LinAngUpdate;

  // ----------------------------------------------------- contruction interface
  NetworkPositionUpdateComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static NetworkPositionUpdateComponentPtr create(Args&& ... a) {
    return std::make_shared<NetworkPositionUpdateComponent>(a...);
  }

  // creates a copy from this
  NetworkPositionUpdateComponentPtr create_copy() const {
    return std::make_shared<NetworkPositionUpdateComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "NetworkPositionUpdateComponent"; }

  void init(bool is_local);

  virtual void accept(SavableObjectVisitor& visitor);
  virtual void update(double time);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void send_update();

  float update_interval_;
  float time_;

  bool is_local_;

  DynamicBodyComponentPtr body_;
};

}

#endif  // SWIFT2D_NETWORK_POSITION_UPDATE_COMPONENT_HPP
