////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LIFE_COMPONENT_HPP
#define SWIFT2D_LIFE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class LifeComponent;
typedef std::shared_ptr<LifeComponent>       LifeComponentPtr;
typedef std::shared_ptr<const LifeComponent> ConstLifeComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL LifeComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Signal<math::uint64> on_killed;
  Signal<float> on_life_change;

  // ---------------------------------------------------------------- properties
  Float Life;
  Float MaxLife;

  Float DamageSourceResetTime;

  Bool Invulnerable;

  // ----------------------------------------------------- contruction interface
  LifeComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static LifeComponentPtr create(Args&& ... a) {
    return std::make_shared<LifeComponent>(a...);
  }

  // creates a copy from this
  LifeComponentPtr create_copy() const {
    return std::make_shared<LifeComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "LifeComponent"; }

  void increase(float amount);

  void decrease(float amount);
  void decrease(float amount, math::uint64 source);

  void reset();

  bool is_dead() const;

  virtual void accept(SavableObjectVisitor& visitor);
  virtual void update(double time);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  math::uint64 damage_source_;
  float        reset_time_;
  bool         dead_;
};

}

#endif  // SWIFT2D_LIFE_COMPONENT_HPP
