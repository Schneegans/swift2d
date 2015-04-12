////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_RESPAWN_COMPONENT_HPP
#define SWIFT2D_RESPAWN_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/events/Scheduler.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class RespawnComponent;
typedef std::shared_ptr<RespawnComponent>       RespawnComponentPtr;
typedef std::shared_ptr<const RespawnComponent> ConstRespawnComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL RespawnComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Signal<> on_respawn;

  // ---------------------------------------------------------------- properties
  Float RespawnTime;

  // ----------------------------------------------------- contruction interface
  RespawnComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static RespawnComponentPtr create(Args&& ... a) {
    return std::make_shared<RespawnComponent>(a...);
  }

  // creates a copy from this
  RespawnComponentPtr create_copy() const {
    return std::make_shared<RespawnComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "RespawnComponent"; }

  virtual void accept(SavableObjectVisitor& visitor);
  virtual void update(double time);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Scheduler scheduler_;
  bool initialized_;
};

}

#endif  // SWIFT2D_RESPAWN_COMPONENT_HPP
