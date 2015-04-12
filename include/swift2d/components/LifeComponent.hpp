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
  Signal<math::uint64, float, math::vec2> on_life_change;

  // ---------------------------------------------------------------- properties
  Float Life;
  Float MaxLife;
  Float AccumulationDelay;
  Float AccumulationThreshold;

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

  void increase(float amount, math::vec2 const& direction = math::vec2());

  void decrease(float amount, math::vec2 const& direction = math::vec2());
  void decrease(float amount, math::uint64 source, math::vec2 const& direction = math::vec2());

  void reset();

  bool is_dead() const;

  virtual void accept(SavableObjectVisitor& visitor);
  virtual void update(double time);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void accumulate_damage(float amount, math::vec2 const& direction);
  void emit_accumulated_damage();

  math::uint64 damage_source_;
  float        reset_time_;
  bool         dead_;

  float       accumulation_time_;
  float       accumulated_damage_;
  float       accumulated_abs_damage_;
  math::vec2  accumulated_direction_;
};

}

#endif  // SWIFT2D_LIFE_COMPONENT_HPP
