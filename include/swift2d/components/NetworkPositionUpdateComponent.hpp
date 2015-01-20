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
  NetworkPositionUpdateComponent(bool is_local=false);

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "NetworkPositionUpdateComponent"; }

  virtual void accept(SavableObjectVisitor& visitor);
  virtual void update(double time);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  float update_interval_;
  float time_;

  bool is_local_;

  DynamicBodyComponentPtr body_;
};

}

#endif  // SWIFT2D_NETWORK_POSITION_UPDATE_COMPONENT_HPP
